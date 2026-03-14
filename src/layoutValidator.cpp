/****************************************************************************
**
** Layout Validator — programmatic grid audit for SY-1000 FloorBoard
**
** Walks every QGridLayout in every QStackedWidget page and checks:
**   1. Column count vs MAX_COLS
**   2. Total grid minimum width vs MAX_GRID_WIDTH
**   3. Label text clipping (text wider than allocated width)
**   4. Widget minimum size vs cell allocation
**   5. Overlapping cell assignments
**
** Usage: call LayoutValidator::validatePage() on any editPage widget,
** then writeReport() to dump results to a file.
**
****************************************************************************/

#include "layoutValidator.h"
#include "customControlLabel.h"
#include "customControlKnob.h"
#include "customControlListMenu.h"
#include "customControlSwitch.h"
#include "customControlDataKnob.h"
#include "customControlSpinBox.h"
#include "customControlParaEQ.h"
#include "customSystemOverride.h"
#include "Preferences.h"
#include <QGridLayout>
#include <QStackedWidget>
#include <QGroupBox>
#include <QLabel>
#include <QFontMetrics>
#include <QDebug>
#include <QSet>
#include <QDateTime>

QString LayoutValidator::classifyWidget(QWidget *widget)
{
    if(!widget) return "null";
    if(qobject_cast<customControlKnob*>(widget)) return "knob";
    if(qobject_cast<customControlDataKnob*>(widget)) return "dataknob";
    if(qobject_cast<customControlListMenu*>(widget)) return "combo";
    if(qobject_cast<customControlSwitch*>(widget)) return "switch";
    if(qobject_cast<customControlSpinBox*>(widget)) return "spinbox";
    if(qobject_cast<customControlParaEQ*>(widget)) return "paraeq";
    if(qobject_cast<customSystemOverride*>(widget)) return "override";
    if(qobject_cast<customControlLabel*>(widget)) return "label";
    if(qobject_cast<QGroupBox*>(widget)) return "groupbox";
    if(qobject_cast<QLabel*>(widget)) return "qlabel";

    QString cls = widget->metaObject()->className();
    if(cls.contains("Divider", Qt::CaseInsensitive)) return "divider";
    if(cls.contains("TabMenu", Qt::CaseInsensitive)) return "tabmenu";
    return cls;
}

QString LayoutValidator::extractLabelText(QWidget *widget)
{
    if(!widget) return QString();

    // All control widgets (knob, combo, switch) have a customControlLabel child named via findChild
    customControlLabel *ccl = widget->findChild<customControlLabel*>();
    if(ccl)
    {
        // The label has a QLabel child with the actual text
        QLabel *ql = ccl->findChild<QLabel*>();
        if(ql) return ql->text();
    }

    // For plain QLabel widgets
    QLabel *ql = qobject_cast<QLabel*>(widget);
    if(ql) return ql->text();

    return QString();
}

int LayoutValidator::measureLabelTextWidth(QWidget *widget)
{
    if(!widget) return 0;

    customControlLabel *ccl = widget->findChild<customControlLabel*>();
    if(ccl)
    {
        return ccl->getLabelWidth();
    }

    QLabel *ql = qobject_cast<QLabel*>(widget);
    if(ql)
    {
        QFontMetrics fm(ql->font());
        return fm.horizontalAdvance(ql->text());
    }

    return 0;
}

int LayoutValidator::getAllocatedLabelWidth(QWidget *widget)
{
    if(!widget) return 0;

    customControlLabel *ccl = widget->findChild<customControlLabel*>();
    if(ccl)
    {
        // textWidth is private, but we can check fixedWidth
        if(ccl->maximumWidth() < QWIDGETSIZE_MAX)
            return ccl->maximumWidth();
    }
    return 0;
}

GridReport LayoutValidator::validateGrid(QGridLayout *grid, const QString &pageName,
                                         int stackId, int pageIndex)
{
    GridReport report;
    report.pageName = pageName;
    report.stackControlId = stackId;
    report.stackPageIndex = pageIndex;
    report.maxCol = -1;
    report.maxRow = -1;
    report.hSpacing = grid->horizontalSpacing();
    report.vSpacing = grid->verticalSpacing();

    Preferences *preferences = Preferences::Instance();
    bool ok;
    double ratio = preferences->getPreferences("Window", "Scale", "ratio").toDouble(&ok);
    if(!ok) ratio = 1.0;

    // Track occupied cells for overlap detection
    QSet<QPair<int,int>> occupiedCells;

    for(int i = 0; i < grid->count(); ++i)
    {
        QLayoutItem *item = grid->itemAt(i);
        if(!item || !item->widget()) continue;

        QWidget *widget = item->widget();
        int row, col, rowSpan, colSpan;
        grid->getItemPosition(i, &row, &col, &rowSpan, &colSpan);

        QString wtype = classifyWidget(widget);

        // Skip system overrides — they're invisible overlays, not real widgets
        if(wtype == "override") continue;

        GridCellInfo cell;
        cell.row = row;
        cell.col = col;
        cell.rowSpan = rowSpan;
        cell.colSpan = colSpan;
        cell.widgetType = wtype;
        cell.objectName = widget->objectName();
        cell.minWidthHint = widget->minimumSizeHint().width();
        cell.minHeightHint = widget->minimumSizeHint().height();
        cell.sizeHintWidth = widget->sizeHint().width();
        cell.sizeHintHeight = widget->sizeHint().height();
        cell.fixedWidth = (widget->minimumWidth() == widget->maximumWidth()) ? widget->minimumWidth() : -1;
        cell.fixedHeight = (widget->minimumHeight() == widget->maximumHeight()) ? widget->minimumHeight() : -1;
        cell.labelText = extractLabelText(widget);
        cell.labelTextPixelWidth = measureLabelTextWidth(widget);
        cell.labelAllocatedWidth = getAllocatedLabelWidth(widget);

        // Track max row/col
        if(col + colSpan - 1 > report.maxCol) report.maxCol = col + colSpan - 1;
        if(row + rowSpan - 1 > report.maxRow) report.maxRow = row + rowSpan - 1;

        // Check for overlapping cells
        for(int r = row; r < row + rowSpan; ++r)
        {
            for(int c = col; c < col + colSpan; ++c)
            {
                QPair<int,int> key(r, c);
                if(occupiedCells.contains(key))
                {
                    report.violations.append(
                        QString("OVERLAP: %1 '%2' at (%3,%4) overlaps existing widget at cell (%5,%6)")
                            .arg(wtype, cell.labelText)
                            .arg(row).arg(col).arg(r).arg(c));
                }
                occupiedCells.insert(key);
            }
        }

        // Check label clipping (non-wrapping case)
        if(cell.labelAllocatedWidth > 0 && cell.labelTextPixelWidth > cell.labelAllocatedWidth)
        {
            report.violations.append(
                QString("LABEL_CLIP: %1 '%2' at (%3,%4) — text is %5px wide but only %6px allocated")
                    .arg(wtype, cell.labelText)
                    .arg(row).arg(col)
                    .arg(cell.labelTextPixelWidth).arg(cell.labelAllocatedWidth));
        }

        report.cells.append(cell);
    }

    report.colCount = report.maxCol + 1;
    report.rowCount = report.maxRow + 1;

    // Compute total minimum grid width
    report.totalMinWidth = 0;
    for(int c = 0; c <= report.maxCol; ++c)
    {
        report.totalMinWidth += grid->columnMinimumWidth(c);
    }
    if(report.maxCol > 0)
    {
        report.totalMinWidth += report.hSpacing * report.maxCol;
    }
    // Add margins
    int ml, mt, mr, mb;
    grid->getContentsMargins(&ml, &mt, &mr, &mb);
    report.totalMinWidth += ml + mr;

    // Compute total minimum grid height
    report.totalMinHeight = 0;
    for(int r = 0; r <= report.maxRow; ++r)
    {
        report.totalMinHeight += grid->rowMinimumHeight(r);
    }
    if(report.maxRow > 0)
    {
        report.totalMinHeight += report.vSpacing * report.maxRow;
    }
    report.totalMinHeight += mt + mb;

    // Check column count violation
    if(report.colCount > MAX_COLS)
    {
        report.violations.append(
            QString("COL_COUNT: grid uses %1 columns (max %2)")
                .arg(report.colCount).arg(MAX_COLS));
    }

    // Check total width violation
    if(report.totalMinWidth > MAX_GRID_WIDTH * ratio)
    {
        report.violations.append(
            QString("GRID_WIDTH: total minimum width %1px exceeds max %2px")
                .arg(report.totalMinWidth).arg(qRound(MAX_GRID_WIDTH * ratio)));
    }

    // Check individual widget minimum size vs allocated cell
    for(const GridCellInfo &cell : report.cells)
    {
        int allocatedWidth = 0;
        for(int c = cell.col; c < cell.col + cell.colSpan; ++c)
        {
            allocatedWidth += grid->columnMinimumWidth(c);
        }
        if(cell.colSpan > 1) allocatedWidth += report.hSpacing * (cell.colSpan - 1);

        if(cell.minWidthHint > 0 && cell.minWidthHint > allocatedWidth && allocatedWidth > 0)
        {
            report.violations.append(
                QString("WIDGET_CLIP: %1 '%2' at (%3,%4) — minSizeHint %5px > cell allocation %6px")
                    .arg(cell.widgetType, cell.labelText)
                    .arg(cell.row).arg(cell.col)
                    .arg(cell.minWidthHint).arg(allocatedWidth));
        }
    }

    return report;
}

void LayoutValidator::findGridLayouts(QWidget *widget, QList<QGridLayout*> &grids,
                                      QList<QPair<int,int>> &ids, int stackId, int pageIndex)
{
    if(!widget) return;

    // Check if this widget has a QGridLayout
    QGridLayout *grid = qobject_cast<QGridLayout*>(widget->layout());
    if(grid && grid->count() > 0)
    {
        grids.append(grid);
        ids.append(QPair<int,int>(stackId, pageIndex));
    }

    // Recurse into QStackedWidgets
    QStackedWidget *stacked = qobject_cast<QStackedWidget*>(widget);
    if(stacked)
    {
        for(int i = 0; i < stacked->count(); ++i)
        {
            findGridLayouts(stacked->widget(i), grids, ids, stackId, i);
        }
    }

    // Recurse into QGroupBoxes
    QGroupBox *gbox = qobject_cast<QGroupBox*>(widget);
    if(gbox)
    {
        QGridLayout *gLayout = qobject_cast<QGridLayout*>(gbox->layout());
        if(gLayout && gLayout->count() > 0)
        {
            grids.append(gLayout);
            ids.append(QPair<int,int>(stackId, pageIndex));
        }
    }

    // Recurse into child widgets
    for(QObject *child : widget->children())
    {
        QWidget *childWidget = qobject_cast<QWidget*>(child);
        if(childWidget && !qobject_cast<QStackedWidget*>(widget))
        {
            // Don't double-recurse into stacked widgets (handled above)
            findGridLayouts(childWidget, grids, ids, stackId, pageIndex);
        }
    }
}

QList<GridReport> LayoutValidator::validatePage(QWidget *page, const QString &pageName)
{
    QList<GridReport> reports;
    if(!page) return reports;

    // Find all QStackedWidgets (these are the stackedFields)
    QList<QStackedWidget*> stackedWidgets = page->findChildren<QStackedWidget*>();

    int stackIdx = 0;
    for(QStackedWidget *sw : stackedWidgets)
    {
        for(int i = 0; i < sw->count(); ++i)
        {
            QWidget *pageWidget = sw->widget(i);
            if(!pageWidget) continue;

            QGridLayout *grid = qobject_cast<QGridLayout*>(pageWidget->layout());
            if(!grid || grid->count() == 0) continue;

            GridReport report = validateGrid(grid, pageName, stackIdx, i);
            if(report.colCount > 0)
            {
                reports.append(report);
            }

            // Also check for nested groupbox grids
            QList<QGroupBox*> groupBoxes = pageWidget->findChildren<QGroupBox*>();
            for(QGroupBox *gb : groupBoxes)
            {
                QGridLayout *gbGrid = qobject_cast<QGridLayout*>(gb->layout());
                if(gbGrid && gbGrid->count() > 0)
                {
                    GridReport gbReport = validateGrid(gbGrid,
                        pageName + "/" + gb->title(), stackIdx, i);
                    if(gbReport.colCount > 0)
                    {
                        reports.append(gbReport);
                    }
                }
            }
        }
        ++stackIdx;
    }

    return reports;
}

void LayoutValidator::writeReport(const QList<GridReport> &reports, const QString &filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "LayoutValidator: cannot open" << filePath << "for writing";
        return;
    }

    QTextStream out(&file);
    out << "# Layout Validation Report\n";
    out << "# Generated: " << QDateTime::currentDateTime().toString(Qt::ISODate) << "\n\n";

    // Summary
    int totalGrids = reports.size();
    int gridsWithViolations = 0;
    int totalViolations = 0;
    int colCountViolations = 0;
    int widthViolations = 0;
    int labelClipViolations = 0;
    int overlapViolations = 0;
    int widgetClipViolations = 0;

    for(const GridReport &r : reports)
    {
        if(!r.violations.isEmpty()) ++gridsWithViolations;
        totalViolations += r.violations.size();
        for(const QString &v : r.violations)
        {
            if(v.startsWith("COL_COUNT")) ++colCountViolations;
            else if(v.startsWith("GRID_WIDTH")) ++widthViolations;
            else if(v.startsWith("LABEL_CLIP")) ++labelClipViolations;
            else if(v.startsWith("OVERLAP")) ++overlapViolations;
            else if(v.startsWith("WIDGET_CLIP")) ++widgetClipViolations;
        }
    }

    out << "## Summary\n";
    out << "Total grids audited: " << totalGrids << "\n";
    out << "Grids with violations: " << gridsWithViolations << "\n";
    out << "Total violations: " << totalViolations << "\n";
    out << "  COL_COUNT: " << colCountViolations << "\n";
    out << "  GRID_WIDTH: " << widthViolations << "\n";
    out << "  LABEL_CLIP: " << labelClipViolations << "\n";
    out << "  WIDGET_CLIP: " << widgetClipViolations << "\n";
    out << "  OVERLAP: " << overlapViolations << "\n\n";

    // Violations detail
    out << "## Violations\n\n";
    for(const GridReport &r : reports)
    {
        if(r.violations.isEmpty()) continue;

        out << "### " << r.pageName
            << " stack=" << r.stackControlId
            << " page=" << r.stackPageIndex
            << " [" << r.colCount << "c x " << r.rowCount << "r"
            << " minW=" << r.totalMinWidth << "px"
            << " hSp=" << r.hSpacing << " vSp=" << r.vSpacing << "]\n";

        for(const QString &v : r.violations)
        {
            out << "  - " << v << "\n";
        }
        out << "\n";
    }

    // Full grid inventory
    out << "## Full Grid Inventory\n\n";
    for(const GridReport &r : reports)
    {
        out << "### " << r.pageName
            << " stack=" << r.stackControlId
            << " page=" << r.stackPageIndex
            << " [" << r.colCount << "c x " << r.rowCount << "r"
            << " minW=" << r.totalMinWidth << "px"
            << " minH=" << r.totalMinHeight << "px"
            << " hSp=" << r.hSpacing << " vSp=" << r.vSpacing
            << (r.violations.isEmpty() ? " OK" : " VIOLATIONS") << "]\n";

        for(const GridCellInfo &c : r.cells)
        {
            out << "  (" << c.row << "," << c.col;
            if(c.rowSpan > 1 || c.colSpan > 1)
                out << " span " << c.rowSpan << "x" << c.colSpan;
            out << ") " << c.widgetType;
            if(!c.labelText.isEmpty())
                out << " \"" << c.labelText << "\"";
            out << " minW=" << c.minWidthHint
                << " sizeW=" << c.sizeHintWidth;
            if(c.fixedWidth > 0)
                out << " fixW=" << c.fixedWidth;
            if(c.labelTextPixelWidth > 0)
                out << " txtW=" << c.labelTextPixelWidth;
            if(c.labelAllocatedWidth > 0)
                out << " allocW=" << c.labelAllocatedWidth;
            out << "\n";
        }
        out << "\n";
    }

    file.close();
    qDebug() << "LayoutValidator: report written to" << filePath;
}

void LayoutValidator::writeSummary(const QList<GridReport> &reports)
{
    int totalViolations = 0;
    for(const GridReport &r : reports)
        totalViolations += r.violations.size();

    if(totalViolations == 0)
    {
        qDebug() << "LayoutValidator: all grids pass validation";
        return;
    }

    qWarning() << "LayoutValidator:" << totalViolations << "violations found across" << reports.size() << "grids";
    for(const GridReport &r : reports)
    {
        for(const QString &v : r.violations)
        {
            qWarning().noquote() << "  " << r.pageName
                                 << " s" << r.stackControlId
                                 << "/p" << r.stackPageIndex
                                 << ": " << v;
        }
    }
}

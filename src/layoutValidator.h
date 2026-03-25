#ifndef LAYOUTVALIDATOR_H
#define LAYOUTVALIDATOR_H

#include <QWidget>
#include <QGridLayout>
#include <QStackedWidget>
#include <QString>
#include <QList>
#include <QFile>
#include <QTextStream>

struct GridCellInfo {
    int row;
    int col;
    int rowSpan;
    int colSpan;
    QString widgetType;      // "knob", "combo", "switch", "label", "divider", etc.
    QString objectName;
    int minWidthHint;        // widget's minimumSizeHint().width()
    int minHeightHint;       // widget's minimumSizeHint().height()
    int sizeHintWidth;       // widget's sizeHint().width()
    int sizeHintHeight;      // widget's sizeHint().height()
    int fixedWidth;          // if setFixedWidth was called, -1 otherwise
    int fixedHeight;         // if setFixedHeight was called, -1 otherwise
    QString labelText;       // extracted label text if available
    int labelTextPixelWidth; // measured text width in pixels
    int labelAllocatedWidth; // the setTextWidth value, or cell width
};

struct GridReport {
    QString pageName;        // e.g. "inst1"
    int stackControlId;      // which stack control
    int stackPageIndex;      // which page within the stacked widget
    int maxCol;              // highest column index used
    int maxRow;              // highest row index used
    int colCount;            // maxCol + 1
    int rowCount;            // maxRow + 1
    int totalMinWidth;       // sum of column minimum widths + spacing
    int totalMinHeight;      // sum of row minimum heights + spacing
    int hSpacing;
    int vSpacing;
    QList<GridCellInfo> cells;
    QStringList violations;  // human-readable violation descriptions
};

class LayoutValidator
{
public:
    // Maximum columns for a uniform grid (matches GRID_COLS in gridConstants.h)
    static constexpr int MAX_COLS = 6;
    // Maximum total grid width in pixels at ratio 1.0
    // (typical panel width is ~850px based on the editor window)
    static constexpr int MAX_GRID_WIDTH = 900;

    // Validate all grids within a widget tree (typically an editPage)
    static QList<GridReport> validatePage(QWidget *page, const QString &pageName);

    // Validate a single QGridLayout
    static GridReport validateGrid(QGridLayout *grid, const QString &pageName,
                                   int stackId, int pageIndex);

    // Write all reports to a file
    static void writeReport(const QList<GridReport> &reports, const QString &filePath);

    // Write a concise summary to stderr
    static void writeSummary(const QList<GridReport> &reports);

private:
    // Recursively find all QGridLayouts in a widget tree
    static void findGridLayouts(QWidget *widget, QList<QGridLayout*> &grids,
                                QList<QPair<int,int>> &ids, int stackId, int pageIndex);

    // Classify a widget by type
    static QString classifyWidget(QWidget *widget);

    // Extract label text from a control widget (knob, combo, switch)
    static QString extractLabelText(QWidget *widget);

    // Measure the pixel width of label text using the widget's font
    static int measureLabelTextWidth(QWidget *widget);

    // Get the allocated text width (from setTextWidth calls)
    static int getAllocatedLabelWidth(QWidget *widget);
};

#endif // LAYOUTVALIDATOR_H

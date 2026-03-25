#!/usr/bin/env python3
"""Generate filmstrip knob PNGs for QML UI.

Produces vertical PNG strips: each frame stacked top-to-bottom.
Frame 0 = min position, last frame = max position.

Renders at 2x for Retina displays — a "48px" knob is actually 96px in the PNG.
"""

import math
import os
from PIL import Image, ImageDraw, ImageFilter

RETINA = 2  # render at 2x, display at 1x in QML with sourceSize


def draw_knob_frame(size, angle_deg, style="default"):
    """Draw a single knob frame at the given pixel size.

    angle_deg: pointer angle in degrees. 0=top, rotates clockwise.
               Range: -135 (min) to +135 (max)
    """
    img = Image.new("RGBA", (size, size), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)

    cx, cy = size / 2, size / 2
    r = size / 2 - 2

    if style == "gain":
        accent = (255, 60, 60)
        body_outer = (80, 80, 85)
        body_inner = (55, 55, 60)
        pointer_color = (255, 60, 60)
        ring_color = (40, 40, 42)
        tick_color = (100, 100, 105)
    else:
        accent = (0, 204, 255)
        body_outer = (60, 62, 65)
        body_inner = (40, 42, 45)
        pointer_color = (0, 204, 255)
        ring_color = (28, 28, 30)
        tick_color = (80, 82, 85)

    # Shadow
    draw.ellipse(
        [cx - r + 1, cy - r + 2, cx + r + 1, cy + r + 2],
        fill=(0, 0, 0, 60)
    )

    # Outer ring
    draw.ellipse([cx - r, cy - r, cx + r, cy + r], fill=ring_color)

    # Arc track (background)
    arc_r = r - 3
    arc_bbox = [cx - arc_r, cy - arc_r, cx + arc_r, cy + arc_r]
    draw.arc(arc_bbox, 135, 45, fill=tick_color, width=max(2, size // 20))

    # Arc track (value)
    if angle_deg > -135:
        end_pil = 135 + (angle_deg + 135) / 270 * 270
        if end_pil > 135:
            draw.arc(arc_bbox, 135, end_pil, fill=accent, width=max(2, size // 20))

    # Knob body
    body_r = r - 6
    draw.ellipse(
        [cx - body_r, cy - body_r, cx + body_r, cy + body_r],
        fill=body_outer
    )
    inner_r = body_r - max(2, size // 25)
    draw.ellipse(
        [cx - inner_r, cy - inner_r - 1, cx + inner_r, cy + inner_r - 1],
        fill=body_inner
    )

    # Pointer line
    angle_rad = math.radians(angle_deg - 90)
    ptr_inner = body_r * 0.25
    ptr_outer = body_r * 0.8
    x1 = cx + ptr_inner * math.cos(angle_rad)
    y1 = cy + ptr_inner * math.sin(angle_rad)
    x2 = cx + ptr_outer * math.cos(angle_rad)
    y2 = cy + ptr_outer * math.sin(angle_rad)
    draw.line([(x1, y1), (x2, y2)], fill=pointer_color, width=max(2, size // 25))

    # Dot at tip
    dot_r = max(1.5, size / 30)
    draw.ellipse([x2 - dot_r, y2 - dot_r, x2 + dot_r, y2 + dot_r], fill=pointer_color)

    return img


def generate_filmstrip(filename, logical_size, num_frames, style="default"):
    """Generate a vertical filmstrip PNG at 2x resolution."""
    px = logical_size * RETINA
    strip = Image.new("RGBA", (px, px * num_frames), (0, 0, 0, 0))

    for i in range(num_frames):
        ratio = i / (num_frames - 1)
        angle = -135 + ratio * 270
        frame = draw_knob_frame(px, angle, style)
        strip.paste(frame, (0, i * px))

    strip.save(filename, optimize=True)
    print(f"  {filename}")
    print(f"    logical: {logical_size}x{logical_size}, {num_frames} frames")
    print(f"    actual:  {px}x{px * num_frames} PNG")


if __name__ == "__main__":
    outdir = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "qml", "knobs")

    print("Generating filmstrip knob PNGs (2x Retina)...")

    # 48px logical = 96px actual — standard knob
    generate_filmstrip(f"{outdir}/knob_48.png", 48, 128, "default")

    # 56px logical = 112px actual — large knob (INST Level)
    generate_filmstrip(f"{outdir}/knob_56.png", 56, 128, "default")

    # 48px gain style
    generate_filmstrip(f"{outdir}/knob_48_gain.png", 48, 128, "gain")

    print("Done!")

#!/usr/bin/env python3
"""Generate filmstrip knob PNGs for QML UI.

Produces vertical PNG strips: each frame stacked top-to-bottom.
Frame 0 = min position, last frame = max position.
"""

import math
from PIL import Image, ImageDraw

def draw_knob_frame(size, angle_deg, style="default"):
    """Draw a single knob frame.

    angle_deg: pointer angle in degrees. 0=top, rotates clockwise.
               Range: -135 (min) to +135 (max)
    """
    img = Image.new("RGBA", (size, size), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)

    cx, cy = size / 2, size / 2
    r = size / 2 - 2  # outer radius

    if style == "gain":
        # Metallic silver knob with red accent
        accent = (255, 60, 60)
        body_outer = (80, 80, 85)
        body_inner = (55, 55, 60)
        pointer_color = (255, 60, 60)
        ring_color = (40, 40, 42)
        tick_color = (100, 100, 105)
    else:
        # Zenology-style dark knob with cyan accent
        accent = (0, 204, 255)
        body_outer = (60, 62, 65)
        body_inner = (40, 42, 45)
        pointer_color = (0, 204, 255)
        ring_color = (28, 28, 30)
        tick_color = (80, 82, 85)

    # Shadow
    shadow_offset = 1
    draw.ellipse(
        [cx - r + shadow_offset, cy - r + shadow_offset + 1,
         cx + r + shadow_offset, cy + r + shadow_offset + 1],
        fill=(0, 0, 0, 80)
    )

    # Outer ring / bezel
    draw.ellipse([cx - r, cy - r, cx + r, cy + r], fill=ring_color)

    # Arc track (background) - from -135 to +135 degrees
    arc_r = r - 3
    arc_bbox = [cx - arc_r, cy - arc_r, cx + arc_r, cy + arc_r]
    # PIL arc uses 0=3 o'clock, goes clockwise. Convert our angles.
    # Our -135 deg (from top) = 225 deg in standard = 225-90=135 in PIL?
    # Actually PIL: 0=right, 90=down, 180=left, 270=up
    # Our angle system: 0=up. So -135 from up = going CCW from up = 225 in standard
    # In PIL coords: up is 270. So -135 from up = 270-135 = 135 PIL degrees
    # And +135 from up = 270+135 = 405 = 45 PIL degrees
    draw.arc(arc_bbox, 135, 45, fill=tick_color, width=2)

    # Arc track (value) - from start to current angle
    if angle_deg > -135:
        # Map angle_deg from [-135, +135] to PIL arc angles
        # -135 -> 135 PIL, +135 -> 405 (45) PIL
        end_pil = 135 + (angle_deg + 135) / 270 * 270
        if end_pil > 135:
            draw.arc(arc_bbox, 135, end_pil, fill=accent, width=2)

    # Knob body (gradient effect with two circles)
    body_r = r - 6
    draw.ellipse(
        [cx - body_r, cy - body_r, cx + body_r, cy + body_r],
        fill=body_outer
    )
    inner_r = body_r - 3
    # Slight offset for 3D effect
    draw.ellipse(
        [cx - inner_r, cy - inner_r - 1, cx + inner_r, cy + inner_r - 1],
        fill=body_inner
    )

    # Pointer line
    angle_rad = math.radians(angle_deg - 90)  # -90 because 0=up in our system
    ptr_inner = body_r * 0.25
    ptr_outer = body_r * 0.85
    x1 = cx + ptr_inner * math.cos(angle_rad)
    y1 = cy + ptr_inner * math.sin(angle_rad)
    x2 = cx + ptr_outer * math.cos(angle_rad)
    y2 = cy + ptr_outer * math.sin(angle_rad)
    draw.line([(x1, y1), (x2, y2)], fill=pointer_color, width=2)

    # Small dot at pointer tip
    dot_r = 1.5
    draw.ellipse([x2 - dot_r, y2 - dot_r, x2 + dot_r, y2 + dot_r], fill=pointer_color)

    return img


def generate_filmstrip(filename, size, num_frames, style="default"):
    """Generate a vertical filmstrip PNG."""
    strip = Image.new("RGBA", (size, size * num_frames), (0, 0, 0, 0))

    for i in range(num_frames):
        ratio = i / (num_frames - 1)
        angle = -135 + ratio * 270  # -135 to +135
        frame = draw_knob_frame(size, angle, style)
        strip.paste(frame, (0, i * size))

    strip.save(filename)
    print(f"  {filename}: {size}x{size}, {num_frames} frames, {size}x{size * num_frames} total")


if __name__ == "__main__":
    outdir = "/Users/bsewell/010-MUSIC-STUDIO/SY-1000/qml/knobs"

    print("Generating filmstrip knob PNGs...")

    # Main knob - 60x60, 128 frames (matches Zenology specs)
    generate_filmstrip(f"{outdir}/knob_default_60x60_128f.png", 60, 128, "default")

    # Gain knob - 69x69, 128 frames (matches GAIN specs)
    generate_filmstrip(f"{outdir}/knob_gain_69x69_128f.png", 69, 128, "gain")

    # Small knob for tight layouts - 40x40, 64 frames
    generate_filmstrip(f"{outdir}/knob_small_40x40_64f.png", 40, 64, "default")

    print("Done!")

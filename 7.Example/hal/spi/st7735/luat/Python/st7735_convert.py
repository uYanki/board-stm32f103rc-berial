

from PIL import Image

inputs = [
    "parrot_128x128.png"
]

for input in inputs:

    filename = input.rsplit('.')[0]

    with open(f"image_{filename}.h", "w") as f:

        img = Image.open(input)

        s = f"static const uint16_t image_{filename}[] = ""{"
        s += f" // {img.width}x{img.height}"
        for y in range(0, img.height):
            s += "\n    "
            for x in range(0, img.width):
                (r, g, b) = img.getpixel((x, y))
                color565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3)
                color565 = ((color565 & 0xFF00) >> 8) | ((color565 & 0xFF) << 8)
                s += " 0x{:04X},".format(color565)
        s = s[:-1] + "};"

        f.write(s)

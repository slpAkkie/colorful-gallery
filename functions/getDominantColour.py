from PyQt6.QtGui import QPixmap


def getDominantColour(pixmap: QPixmap) -> dict:
    res = 128
    if pixmap.width() > pixmap.height():
        if pixmap.width() > res:
            pixmap = pixmap.scaledToWidth(res)
    else:
        if pixmap.height() > res:
            pixmap = pixmap.scaledToHeight(res)

    image = pixmap.toImage()
    step = 4
    width = image.width()
    height = image.height()
    hue_dict = {}
    saturation_dict = {}
    lightness_dict = {}

    def incValue(dictionary: dict, value):
        _ = value % step
        value = value - _

        if _ > step / 2:
            value += step

        if dictionary.get(value) is None:
            dictionary[value] = 1
        else:
            dictionary[value] += 1

    for x in range(0, width):
        for y in range(0, height):
            saturation: int = image.pixelColor(x, y).hslSaturation()
            lightness: int = image.pixelColor(x, y).lightness()
            if saturation <= 5 or lightness <= 5 or lightness / 2.2 > saturation:
                continue

            incValue(hue_dict, image.pixelColor(x, y).hslHue())
            incValue(saturation_dict, saturation)
            incValue(lightness_dict, lightness)

    hue_dict_keys = list(hue_dict.keys())
    saturation_dict_keys = list(saturation_dict.keys())
    lightness_dict_keys = list(lightness_dict.keys())
    if len(hue_dict_keys) == 0:
        return {
            'h': "",
            's': "",
            'l': ""
        }

    hue_dict_values = list(hue_dict.values())
    saturation_dict_values = list(saturation_dict.values())
    lightness_dict_values = list(lightness_dict.values())

    return {
        'h': hue_dict_keys[hue_dict_values.index(max(hue_dict_values))],
        's': saturation_dict_keys[saturation_dict_values.index(max(saturation_dict_values))],
        'l': lightness_dict_keys[lightness_dict_values.index(max(lightness_dict_values))]
    }

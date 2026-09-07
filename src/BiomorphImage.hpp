#ifndef BIOMORPH_IMAGE_HPP
#define BIOMORPH_IMAGE_HPP

#include <cstdint>
#include <vector>
#include <QMetaType>

struct BiomorphImage
{
    int width = 0;
    int height = 0;

    // RGBA, 8 bits per channel.
    std::vector<std::uint8_t> pixels;

    int stride() const
    {
        return width * 4;
    }

    bool isValid() const
    {
        return width > 0 && height > 0 && pixels.size() == static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 4;
    }
};

Q_DECLARE_METATYPE(BiomorphImage)

#endif
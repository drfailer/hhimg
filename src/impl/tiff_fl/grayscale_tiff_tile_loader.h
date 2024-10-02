#ifndef GRAYSCALE_TIFF_TILE_LOADER_H
#define GRAYSCALE_TIFF_TILE_LOADER_H
#include <hhimg/hhimg.h>
#include <tiffio.h>

template <class DataType>
class GrayscaleTiffTileLoader
    : public fl::AbstractTileLoader<hhimg::fld::View<DataType>> {
    TIFF *tiff_ = nullptr; ///< Tiff file pointer

    std::vector<size_t>
        fullDims_{}, ///< File dimensions to the least to most dense
        tileDims_{}; ///< Tile dimensions to the least to most dense

    std::vector<std::string>
        dimNames_{}; ///< Dimensions name to the least to most dense

    short sampleFormat_ = 0, ///< Sample format as defined by libtiff
        bitsPerSample_ = 0;  ///< Bit Per Sample as defined by libtiff

  public:
    /// @brief GrayscaleTiffTileLoader unique constructor
    /// @param numberThreads Number of threads associated
    /// @param filePath Path of tiff file
    GrayscaleTiffTileLoader(size_t numberThreads, std::string const &filePath)
        : fl::AbstractTileLoader<hhimg::fld::View<DataType>>(
              "GrayscaleTiffTileLoader", filePath, numberThreads) {
        short samplesPerPixel = 0;

        // Open the file
        tiff_ = TIFFOpen(filePath.c_str(), "r");
        if (tiff_ != nullptr) {
            if (TIFFIsTiled(tiff_) == 0) {
                throw(std::runtime_error(
                    "Tile Loader ERROR: The file is not tiled."));
            }

            uint32_t fullHeight = 0, ///< Full height in pixel
                fullWidth = 0,       ///< Full width in pixel
                tileHeight = 0,      ///< Tile height
                tileWidth = 0;       ///< Tile width

            // Load/parse header
            TIFFGetField(tiff_, TIFFTAG_IMAGEWIDTH, &fullWidth);
            TIFFGetField(tiff_, TIFFTAG_IMAGELENGTH, &fullHeight);
            TIFFGetField(tiff_, TIFFTAG_TILEWIDTH, &tileWidth);
            TIFFGetField(tiff_, TIFFTAG_TILELENGTH, &tileHeight);
            TIFFGetField(tiff_, TIFFTAG_SAMPLESPERPIXEL, &samplesPerPixel);
            TIFFGetField(tiff_, TIFFTAG_BITSPERSAMPLE, &(this->bitsPerSample_));
            TIFFGetField(tiff_, TIFFTAG_SAMPLEFORMAT, &(this->sampleFormat_));

            fullDims_ = {fullHeight, fullWidth};
            tileDims_ = {tileHeight, tileWidth};
            dimNames_ = {"Height", "Width"};

            // Test if the file is greyscale
            if (samplesPerPixel != 1) {
                std::stringstream message;
                message << "Tile Loader ERROR: The file is not greyscale: "
                           "SamplesPerPixel = "
                        << samplesPerPixel << ".";
                throw(std::runtime_error(message.str()));
            }
            // Interpret undefined data format as unsigned integer data
            if (sampleFormat_ < 1 || sampleFormat_ > 3) {
                sampleFormat_ = 1;
            }
        } else {
            throw(std::runtime_error(
                "Tile Loader ERROR: The file can not be opened."));
        }
    }

    /// @brief GrayscaleTiffTileLoader destructor
    ~GrayscaleTiffTileLoader() override {
        if (tiff_) {
            TIFFClose(tiff_);
            tiff_ = nullptr;
        }
    }

    /// @brief Copy Method for the GrayscaleTiffTileLoader
    /// @return Return a copy of the current GrayscaleTiffTileLoader
    std::shared_ptr<fl::AbstractTileLoader<hhimg::fld::View<DataType>>>
    copyTileLoader() override {
        return std::make_shared<GrayscaleTiffTileLoader<DataType>>(
            this->numberThreads(), this->filePath());
    }

    /// @brief Load a tile from the file
    /// @param tile Buffer to fill (already allocated)
    /// @param index Tile index
    /// @param level Tile level (unused)
    void loadTileFromFile(std::shared_ptr<std::vector<DataType>> tile,
                          std::vector<size_t> const &index,
                          [[maybe_unused]] size_t level) override {
        tdata_t tiffTile = nullptr;
        tiffTile = _TIFFmalloc(TIFFTileSize(tiff_));
        TIFFReadTile(tiff_, tiffTile, uint32_t(index.at(1) * tileDims_.at(1)),
                     uint32_t(index.at(0) * tileDims_.at(0)), 0, 0);
        std::stringstream message;
        switch (sampleFormat_) {
        case 1:
            switch (bitsPerSample_) {
            case 8:
                loadTile<uint8_t>(tiffTile, tile);
                break;
            case 16:
                loadTile<uint16_t>(tiffTile, tile);
                break;
            case 32:
                loadTile<uint32_t>(tiffTile, tile);
                break;
            case 64:
                loadTile<uint64_t>(tiffTile, tile);
                break;
            default:
                message
                    << "Tile Loader ERROR: The data format is not supported "
                       "for unsigned integer, number bits per pixel = "
                    << bitsPerSample_;
                throw(std::runtime_error(message.str()));
            }
            break;
        case 2:
            switch (bitsPerSample_) {
            case 8:
                loadTile<int8_t>(tiffTile, tile);
                break;
            case 16:
                loadTile<int16_t>(tiffTile, tile);
                break;
            case 32:
                loadTile<int32_t>(tiffTile, tile);
                break;
            case 64:
                loadTile<int64_t>(tiffTile, tile);
                break;
            default:
                message
                    << "Tile Loader ERROR: The data format is not supported "
                       "for signed integer, number bits per pixel = "
                    << bitsPerSample_;
                throw(std::runtime_error(message.str()));
            }
            break;
        case 3:
            switch (bitsPerSample_) {
            case 8:
            case 16:
            case 32:
                loadTile<float>(tiffTile, tile);
                break;
            case 64:
                loadTile<double>(tiffTile, tile);
                break;
            default:
                message << "Tile Loader ERROR: The data format is not "
                           "supported for float, number bits per pixel = "
                        << bitsPerSample_;
                throw(std::runtime_error(message.str()));
            }
            break;
        default:
            message << "Tile Loader ERROR: The data format is not supported, "
                       "sample format = "
                    << sampleFormat_;
            throw(std::runtime_error(message.str()));
        }

        _TIFFfree(tiffTile);
    }

    /// @brief Number of file/tile dimensions
    /// @details We consider only planar 2D grayscale images. So the two
    /// dimensions are the height and width. If we were to open this loader to
    /// RGB images, the number of channels would be a third dimension.
    /// @return 2
    [[nodiscard]] size_t nbDims() const override { return 2; }

    /// @brief Pyramidal level accessor
    /// @return 1
    [[nodiscard]] size_t nbPyramidLevels() const override { return 1; }

    /// @brief Dimensions of the file, from the least to the most dense (Height,
    /// Width)
    /// @param level Pyramidal level (unused)
    /// @return A reference to the dimensions of the file, from the least to the
    /// most dense (Height, Width)
    [[nodiscard]] std::vector<size_t> const &
    fullDims([[maybe_unused]] std::size_t level) const override {
        return fullDims_;
    }

    /// @brief Dimensions of the tile, from the least to the most dense (Height,
    /// Width)
    /// @param level Pyramidal level (unused)
    /// @return A reference to the dimensions of the tile, from the least to the
    /// most dense (Height, Width)
    [[nodiscard]] std::vector<size_t> const &
    tileDims([[maybe_unused]] std::size_t level) const override {
        return tileDims_;
    }

    /// @brief Dimensions name from the least to the most dense (Height, Width)
    /// @return A reference to the dimensions name from the least to the most
    /// dense (Height, Width)
    [[nodiscard]] std::vector<std::string> const &dimNames() const override {
        return dimNames_;
    }

  private:
    /// @brief Private function to cast and copy the values between the buffer
    /// from libtiff and the view
    /// @tparam FileType Type inside the file
    /// @param src Piece of memory coming from libtiff
    /// @param dest Piece of memory to fill
    template <typename FileType>
    void loadTile(tdata_t src, std::shared_ptr<std::vector<DataType>> &dest) {
        static uint32_t tileSize = uint32_t(tileDims_.at(0) * tileDims_.at(1));
        for (uint32_t i = 0; i < tileSize; ++i) {
            dest->data()[i] = (DataType)((FileType *)(src))[i];
        }
    }
};

#endif

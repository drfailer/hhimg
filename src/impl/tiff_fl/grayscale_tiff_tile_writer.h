#ifndef GRAYSCALE_TIFF_TILE_WRITER_H
#define GRAYSCALE_TIFF_TILE_WRITER_H
#include "grayscale_tiff_tile_loader.h"
#include <tiffio.h>

template <class DataType> class GrayscaleTiffTileWriter {
    TIFF *tiff_ = nullptr; ///< Tiff file pointer

    std::vector<size_t>
        tileDims_{}; ///< Tile dimensions to the least to most dense

    short sampleFormat_ = 0, ///< Sample format as defined by libtiff
        bitsPerSample_ = 0;  ///< Bit Per Sample as defined by libtiff

  public:
    /// @brief GrayscaleTiffTileWriter unique constructor
    /// @param numberThreads Number of threads associated
    /// @param filePath Path of tiff file
    GrayscaleTiffTileWriter(
        std::string const &filePath,
        std::shared_ptr<GrayscaleTiffTileLoader<DataType>> tileLoader)
        : tileDims_(tileLoader->tileDims(0)),
          sampleFormat_(tileLoader->sampleFormat()),
          bitsPerSample_(tileLoader->bitsPerSample()) {
        short samplesPerPixel = 1;

        // Open the file
        tiff_ = TIFFOpen(filePath.c_str(), "w+");
        if (tiff_ != nullptr) {
            uint32_t fullHeight =
                         tileLoader->fullDim(0),     ///< Full height in pixel
                fullWidth = tileLoader->fullDim(1),  ///< Full width in pixel
                tileHeight = tileLoader->tileDim(0), ///< Tile height
                tileWidth = tileLoader->tileDim(1);  ///< Tile width

            // Load/parse header
            TIFFSetField(tiff_, TIFFTAG_IMAGEWIDTH, fullWidth);
            TIFFSetField(tiff_, TIFFTAG_IMAGELENGTH, fullHeight);
            TIFFSetField(tiff_, TIFFTAG_TILEWIDTH, tileWidth);
            TIFFSetField(tiff_, TIFFTAG_TILELENGTH, tileHeight);
            TIFFSetField(tiff_, TIFFTAG_SAMPLESPERPIXEL, samplesPerPixel);
            TIFFSetField(tiff_, TIFFTAG_BITSPERSAMPLE, this->bitsPerSample_);
            TIFFSetField(tiff_, TIFFTAG_SAMPLEFORMAT, this->sampleFormat_);
        } else {
            throw(std::runtime_error(
                "Tile Loader ERROR: The file can not be opened."));
        }
    }

    /// @brief GrayscaleTiffTileWriter destructor
    ~GrayscaleTiffTileWriter() {
        if (tiff_) {
            TIFFClose(tiff_);
            tiff_ = nullptr;
        }
    }

    void writeTileToFile(std::shared_ptr<hhimg::fld::View<DataType>> tile) {
        tdata_t tiffTile = nullptr;
        tiffTile = _TIFFmalloc(TIFFTileSize(tiff_));
        std::stringstream message;
        std::vector<size_t> index = tile->indexCentralTile();
        tileDims_ = tile->tileDims();

        switch (sampleFormat_) {
        case 1:
            switch (bitsPerSample_) {
            case 8:
                loadTile<uint8_t>(tile, tiffTile);
                break;
            case 16:
                loadTile<uint16_t>(tile, tiffTile);
                break;
            case 32:
                loadTile<uint32_t>(tile, tiffTile);
                break;
            case 64:
                loadTile<uint64_t>(tile, tiffTile);
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
                loadTile<int8_t>(tile, tiffTile);
                break;
            case 16:
                loadTile<int16_t>(tile, tiffTile);
                break;
            case 32:
                loadTile<int32_t>(tile, tiffTile);
                break;
            case 64:
                loadTile<int64_t>(tile, tiffTile);
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
                loadTile<float>(tile, tiffTile);
                break;
            case 64:
                loadTile<double>(tile, tiffTile);
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
        TIFFWriteTile(tiff_, tiffTile, uint32_t(index.at(1) * tileDims_.at(1)),
                      uint32_t(index.at(0) * tileDims_.at(0)), tile->level(), 0);
        _TIFFfree(tiffTile);
    }

  private:
    /// @brief Private function to cast and copy the values between the buffer
    /// from libtiff and the view
    /// @tparam FileType Type inside the file
    /// @param src Piece of memory to write
    /// @param dest Piece of memory from libtiff to fill
    template <typename FileType>
    void loadTile(std::shared_ptr<hhimg::fld::View<DataType>> &src, tdata_t dest) {
        static uint32_t tileSize = uint32_t(tileDims_.at(0) * tileDims_.at(1));
        DataType *data = src->viewData()->data();
        for (uint32_t i = 0; i < tileSize; ++i) {
            ((FileType *)(dest))[i] = (FileType)data[i];
        }
    }
};

#endif

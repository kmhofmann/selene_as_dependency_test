/**
 * This project serves mainly as a test to compile a stand-alone program that uses the Selene library.
 */

#include <selene/base/io/FileReader.hpp>
#include <selene/base/io/FileWriter.hpp>

#include <selene/img/pixel/PixelTypeAliases.hpp>
#include <selene/img/typed/ImageTypeAliases.hpp>
#include <selene/img/interop/DynImageToImage.hpp>
#include <selene/img/interop/ImageToDynImage.hpp>

#include <selene/img_io/jpeg/Read.hpp>
#include <selene/img_io/jpeg/Write.hpp>
#include <selene/img_io/png/Read.hpp>
#include <selene/img_io/png/Write.hpp>
#include <selene/img_io/tiff/Read.hpp>
#include <selene/img_io/tiff/Write.hpp>

#include <selene/img_ops/Algorithms.hpp>

#include <iostream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

int main()
{
  auto replace_extension = [](const fs::path& path, const std::string& ext) {
    auto new_path = path.filename();
    new_path.replace_extension(ext);
    return new_path;
  };

  const auto jpeg_filename = fs::path{"../bike_duck.jpg"};
  const auto png_filename = fs::path{"../de_waag.png"};
  const auto tiff_filename = fs::path{"../stickers_lzw.tif"};

  std::cout << "Reading JPEG image...\n";
  auto img_jpeg = sln::read_jpeg(sln::FileReader(jpeg_filename.string()));
  std::cout << "img_jpeg"
      << ": w=" << img_jpeg.width() << ", h=" << img_jpeg.height()
      << ", c=" << img_jpeg.nr_channels() << ", b=" << img_jpeg.nr_bytes_per_channel() << '\n';

  std::cout << "Reading PNG image...\n";
  sln::PNGDecompressionOptions png_options{false};
  auto img_png = sln::read_png(sln::FileReader(png_filename.string()), png_options);
  std::cout << "img_png"
      << ": w=" << img_png.width() << ", h=" << img_png.height()
      << ", c=" << img_png.nr_channels() << ", b=" << img_png.nr_bytes_per_channel() << '\n';

  std::cout << "Reading TIFF image...\n";
  auto img_tiff = sln::read_tiff(sln::FileReader(tiff_filename.string()));
  std::cout << "img_tiff"
      << ": w=" << img_tiff.width() << ", h=" << img_tiff.height()
      << ", c=" << img_tiff.nr_channels() << ", b=" << img_tiff.nr_bytes_per_channel() << '\n';

  std::cout << "Converting 16-bit image into 8-bit image...\n";
  auto to_8bit = [](const sln::PixelRGB_16u& px) { return sln::PixelRGB_8u{px[0] >> 8, px[1] >> 8, px[2] >> 8}; };
  const auto img_png_view_typed = sln::to_image_view<sln::PixelRGB_16u>(img_png);
  const auto img_png_8bit_typed = sln::transform_pixels<sln::PixelRGB_8u>(img_png_view_typed, to_8bit);
  const auto img_png_8bit = sln::to_dyn_image_view(img_png_8bit_typed);

  std::cout << "Writing images as JPEG...\n";
  sln::write_jpeg(img_jpeg, sln::FileWriter(replace_extension(jpeg_filename, ".jpg")));
  sln::write_jpeg(img_png_8bit, sln::FileWriter(replace_extension(png_filename, ".jpg")));
  sln::write_jpeg(img_tiff, sln::FileWriter(replace_extension(tiff_filename, ".jpg")));

  std::cout << "Writing images as PNG...\n";
  sln::write_png(img_jpeg, sln::FileWriter(replace_extension(jpeg_filename, ".png")));
  sln::write_png(img_png, sln::FileWriter(replace_extension(png_filename, ".png")));
  sln::write_png(img_tiff, sln::FileWriter(replace_extension(tiff_filename, ".png")));

  std::cout << "Writing images as TIFF...\n";
  sln::write_tiff(img_jpeg, sln::FileWriter(replace_extension(jpeg_filename, ".tif")));
  sln::write_tiff(img_png, sln::FileWriter(replace_extension(png_filename, ".tif")));
  sln::write_tiff(img_tiff, sln::FileWriter(replace_extension(tiff_filename, ".tif")));

  std::cout << "Done.\n";
  return 0;
}
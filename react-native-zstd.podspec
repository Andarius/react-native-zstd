require "json"

package = JSON.parse(File.read(File.join(__dir__, "package.json")))

Pod::Spec.new do |s|
  s.name         = "react-native-zstd"
  s.version      = package["version"]
  s.summary      = package["description"]
  s.homepage     = package["homepage"]
  s.license      = package["license"]
  s.authors      = package["author"]

  s.platforms    = { :ios => min_ios_version_supported }
  s.source       = { :git => "https://github.com/Andarius/react-native-zstd.git", :tag => "#{s.version}" }

  s.source_files = [
    "ios/**/*.{swift}",
    "ios/**/*.{h,m,mm}",
    "cpp/**/*.{h,cpp}",
    "externals/zstd/lib/zstd.h",
    "externals/zstd/lib/common/*.{h,c}",
    "externals/zstd/lib/compress/*.{h,c}",
    "externals/zstd/lib/decompress/*.{h,c,S}"
  ]

  s.dependency 'React-jsi'
  s.dependency 'React-callinvoker'

  load 'nitrogen/generated/ios/react-native-zstd+autolinking.rb'
  add_nitrogen_files(s)

  install_modules_dependencies(s)
end

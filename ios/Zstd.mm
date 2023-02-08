#import "Zstd.h"


#import <React/RCTBridge+Private.h>
#import <React/RCTUtils.h>
#import <jsi/jsi.h>

#import "../cpp/ZstdHostObject.h"

@implementation Zstd


RCT_EXPORT_MODULE(Zstd)

RCT_EXPORT_BLOCKING_SYNCHRONOUS_METHOD(install) {
  NSLog(@"Installing JSI bindings for react-native-zstd...");

  RCTBridge* bridge = [RCTBridge currentBridge];
  RCTCxxBridge* cxxBridge = (RCTCxxBridge*)bridge;

  if (cxxBridge == nil) {
    return @false;
  }

  using namespace facebook;

  auto jsiRuntime = (jsi::Runtime*)cxxBridge.runtime;
  if (jsiRuntime == nil) {
    return @false;
  }
  auto& runtime = *jsiRuntime;

  auto hostObject = std::make_shared<rnzstd::ZstdHostObject>();
  auto object = jsi::Object::createFromHostObject(runtime, hostObject);
  runtime.global().setProperty(runtime, "__ZSTDProxy", std::move(object));

  NSLog(@"Successfully installed JSI bindings for react-native-zstd");
  return @true;
}




@end

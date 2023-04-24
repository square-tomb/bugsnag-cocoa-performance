//
//  NetworkInstrumentation.h
//  BugsnagPerformance
//
//  Created by Karl Stenerud on 14.10.22.
//

#import <Foundation/Foundation.h>
#import "../Tracer.h"
#import "../Configurable.h"
#import "../Startable.h"

@interface BSGURLSessionPerformanceDelegate : NSObject
@end

namespace bugsnag {
class Tracer;

class NetworkInstrumentation: public Configurable, public Startable {
public:
    NetworkInstrumentation(std::shared_ptr<Tracer> tracer) noexcept
    : isEnabled(false)
    , tracer_(tracer)
    {}
    virtual ~NetworkInstrumentation() {}

    void configure(BugsnagPerformanceConfiguration * _Nonnull config) noexcept;
    void start() noexcept;
    
private:
    bool isEnabled{false};
    BSGURLSessionPerformanceDelegate * _Nullable delegate_;
    std::shared_ptr<Tracer> tracer_;
};
}

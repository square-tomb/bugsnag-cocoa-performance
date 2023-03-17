//
//  SpanData.h
//  BugsnagPerformance
//
//  Created by Nick Dowell on 06/10/2022.
//

#pragma once

#import <BugsnagPerformance/BugsnagPerformanceSpan.h>

#import "IdGenerator.h"
#import "SpanKind.h"

namespace bugsnag {
/**
 * SpanData is a representation of all data collected by a span.
 */
class SpanData {
public:
    SpanData(NSString *name, CFAbsoluteTime startTime) noexcept;
    
    SpanData(const SpanData&) = delete;
    
    void addAttributes(NSDictionary *attributes) noexcept;
    
    void updateSamplingProbability(double value) noexcept;
    
    TraceId traceId{0};
    SpanId spanId{0};
    NSString *name{nil};
    SpanKind kind{SPAN_KIND_INTERNAL};
    NSMutableDictionary *attributes{nil};
    double samplingProbability{0};
    CFAbsoluteTime startTime{0};
    CFAbsoluteTime endTime{0};
};
}

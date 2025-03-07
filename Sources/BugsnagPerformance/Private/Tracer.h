//
//  Tracer.h
//  BugsnagPerformance
//
//  Created by Nick Dowell on 23/09/2022.
//

#pragma once

#import <BugsnagPerformance/BugsnagPerformanceConfiguration.h>
#import <BugsnagPerformance/BugsnagPerformanceViewType.h>
#import "Span.h"
#import "Sampler.h"
#import "Batch.h"
#import "SpanOptions.h"
#import "PhasedStartup.h"
#import "SpanAttributesProvider.h"
#import "SpanStackingHandler.h"

#import <memory>

namespace bugsnag {
// https://opentelemetry.io/docs/reference/specification/trace/api/#tracer

/**
 * Tracer starts all spans, then samples them and routes them to the batch when they end.
 */
class Tracer: public PhasedStartup {
public:
    Tracer(std::shared_ptr<SpanStackingHandler> spanContextStack,
           std::shared_ptr<Sampler> sampler,
           std::shared_ptr<Batch> batch,
           void (^onSpanStarted)()) noexcept;
    ~Tracer() {};

    void earlyConfigure(BSGEarlyConfiguration *) noexcept;
    void earlySetup() noexcept {}
    void configure(BugsnagPerformanceConfiguration *configuration) noexcept;
    void start() noexcept;

    void setOnViewLoadSpanStarted(std::function<void(NSString *)> onViewLoadSpanStarted) noexcept {
        onViewLoadSpanStarted_ = onViewLoadSpanStarted;
    }

    BugsnagPerformanceSpan *startAppStartSpan(NSString *name, SpanOptions options) noexcept;

    BugsnagPerformanceSpan *startCustomSpan(NSString *name, SpanOptions options) noexcept;

    BugsnagPerformanceSpan *startViewLoadSpan(BugsnagPerformanceViewType viewType,
                                              NSString *className,
                                              SpanOptions options) noexcept;

    BugsnagPerformanceSpan *startNetworkSpan(NSURL *url, NSString *httpMethod, SpanOptions options) noexcept;

    BugsnagPerformanceSpan *startViewLoadPhaseSpan(NSString *className,
                                                   NSString *phase,
                                                   BugsnagPerformanceSpan *parentContext) noexcept;

    void cancelQueuedSpan(BugsnagPerformanceSpan *span) noexcept;

    void onPrewarmPhaseEnded(void) noexcept;

private:
    Tracer() = delete;
    std::shared_ptr<Sampler> sampler_;
    std::shared_ptr<SpanStackingHandler> spanStackingHandler_;

    std::atomic<bool> isCapturingEarlyNetworkSpans_{true};
    std::mutex earlyNetworkSpansMutex_;
    NSMutableArray<BugsnagPerformanceSpan *> *earlyNetworkSpans_;

    std::atomic<bool> willDiscardPrewarmSpans_{false};
    std::mutex prewarmSpansMutex_;
    NSMutableArray<BugsnagPerformanceSpan *> *prewarmSpans_;

    std::shared_ptr<Batch> batch_;
    void (^onSpanStarted_)(){ ^(){} };
    std::function<void(NSString *)> onViewLoadSpanStarted_{ [](NSString *){} };
    BugsnagPerformanceNetworkRequestCallback networkRequestCallback_ {
        ^BugsnagPerformanceNetworkRequestInfo * _Nonnull(BugsnagPerformanceNetworkRequestInfo * _Nonnull info) {
            return info;
        }
    };

    BugsnagPerformanceSpan *startSpan(NSString *name, SpanOptions options, BSGFirstClass defaultFirstClass) noexcept;
    void trySampleAndAddSpanToBatch(std::shared_ptr<SpanData> spanData);
    void markEarlyNetworkSpan(BugsnagPerformanceSpan *span) noexcept;
    void markPrewarmSpan(BugsnagPerformanceSpan *span) noexcept;
    void endEarlyNetworkSpansPhase() noexcept;
};
}

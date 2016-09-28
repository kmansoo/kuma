/* Copyright (c) 2016, Fengping Bao <jamol@live.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef __Http2Response_H__
#define __Http2Response_H__

#include "kmdefs.h"
#include "H2ConnectionImpl.h"
#include "http/HttpResponseImpl.h"

KUMA_NS_BEGIN

class Http2Response : public KMObject, public HttpResponse::Impl, public DestroyDetector
{
public:
    Http2Response(EventLoop::Impl* loop, std::string ver);
    
    KMError attachStream(H2Connection::Impl* conn, uint32_t streamId) override;
    void addHeader(std::string name, std::string value) override;
    KMError sendResponse(int status_code, const std::string& desc, const std::string& ver) override;
    int sendData(const uint8_t* data, size_t len) override;
    KMError close() override;
    
protected:
    void onHeaders(const HeaderVector &headers, bool endheaders, bool endSteam);
    void onData(uint8_t *data, size_t len, bool endSteam);
    void onRSTStream(int err);
    void onWrite();
    
private:
    void cleanup();
    size_t buildHeaders(int status_code, HeaderVector &headers);
    
private:
    EventLoop::Impl*        loop_;
    H2Connection::Impl*     conn_ { nullptr };
    H2StreamPtr             stream_;
};

KUMA_NS_END

#endif /* __H2Response_H__ */

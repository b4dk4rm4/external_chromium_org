// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_PAGE_CAPTURE_PAGE_CAPTURE_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_PAGE_CAPTURE_PAGE_CAPTURE_API_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "chrome/browser/extensions/extension_function.h"
#include "chrome/common/extensions/api/page_capture.h"
#include "webkit/common/blob/shareable_file_reference.h"

namespace base {
class FilePath;
}

namespace content {
class WebContents;
}

namespace extensions {

class PageCaptureSaveAsMHTMLFunction : public AsyncExtensionFunction {
 public:
  PageCaptureSaveAsMHTMLFunction();

  // Test specific delegate used to test that the temporary file gets deleted.
  class TestDelegate {
   public:
    // Called on the UI thread when the temporary file that contains the
    // generated data has been created.
    virtual void OnTemporaryFileCreated(const base::FilePath& temp_file) = 0;
  };
  static void SetTestDelegate(TestDelegate* delegate);

 private:
  virtual ~PageCaptureSaveAsMHTMLFunction();
  virtual bool RunImpl() OVERRIDE;
  virtual bool OnMessageReceivedFromRenderView(
      const IPC::Message& message) OVERRIDE;

  // Called on the file thread.
  void CreateTemporaryFile();

  // Called on the UI thread.
  void TemporaryFileCreated(bool success);
  void ReturnFailure(const std::string& error);
  void ReturnSuccess(int64 file_size);

  // Callback called once the MHTML generation is done.
  void MHTMLGenerated(const base::FilePath& file_path, int64 mhtml_file_size);

  // Returns the WebContents we are associated with, NULL if it's been closed.
  content::WebContents* GetWebContents();

  scoped_ptr<extensions::api::page_capture::SaveAsMHTML::Params> params_;

  // The path to the temporary file containing the MHTML data.
  base::FilePath mhtml_path_;

  // The file containing the MHTML.
  scoped_refptr<webkit_blob::ShareableFileReference> mhtml_file_;

  DECLARE_EXTENSION_FUNCTION("pageCapture.saveAsMHTML", PAGECAPTURE_SAVEASMHTML)
};

}  // namespace extensions

#endif  // CHROME_BROWSER_EXTENSIONS_API_PAGE_CAPTURE_PAGE_CAPTURE_API_H_

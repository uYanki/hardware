/*
 * Copyright (C) 2008 ZXing authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.example.zxing;
 
import com.doit.carset.R;
import com.google.zxing.Result;
import com.google.zxing.client.result.ParsedResult;

import android.app.Activity;

/**
 * Handles generic products which are not books.
 *
 * @author dswitkin@google.com (Daniel Switkin)
 */
public final class ProductResultHandler extends ResultHandler {

  public ProductResultHandler(Activity activity, ParsedResult result, Result rawResult) {
    super(activity, result, rawResult);
//    showGoogleShopperButton(new View.OnClickListener() {
//      @Override
//      public void onClick(View view) {
//        ProductParsedResult productResult = (ProductParsedResult) getResult();
////        openGoogleShopper(productResult.getNormalizedProductID());
//      }
//    });
  }

//  @Override
//  public int getButtonCount() {
////    return hasCustomProductSearch() ? buttons.length : buttons.length - 1;
//	  return 0;
//  }
//
//  @Override
//  public int getButtonText(int index) {
//    return buttons[index];
//  }
//
//  @Override
//  public void handleButtonPress(int index) {
//    ProductParsedResult productResult = (ProductParsedResult) getResult();
//    switch (index) {
//      case 0:
////        openProductSearch(productResult.getNormalizedProductID());
//        break;
//      case 1:
////        webSearch(productResult.getNormalizedProductID());
//        break;
//      case 2:
////        openURL(fillInCustomSearchURL(productResult.getNormalizedProductID()));
//        break;
//    }
//  }

  @Override
  public int getDisplayTitle() {
    return R.string.result_product;
  }
}

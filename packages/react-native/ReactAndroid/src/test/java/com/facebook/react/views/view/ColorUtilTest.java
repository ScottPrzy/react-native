/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.react.views.view;

import static org.junit.Assert.*;

import android.graphics.PixelFormat;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.robolectric.RobolectricTestRunner;

/** Based on Fresco's DrawableUtilsTest (https://github.com/facebook/fresco). */
@RunWith(RobolectricTestRunner.class)
@Ignore // TODO T110934492
public class ColorUtilTest {

  @Test
  public void testMultiplyColorAlpha() {
    assertEquals(0x00123456, ColorUtil.multiplyColorAlpha(0xC0123456, 0));
    assertEquals(0x07123456, ColorUtil.multiplyColorAlpha(0xC0123456, 10));
    assertEquals(0x96123456, ColorUtil.multiplyColorAlpha(0xC0123456, 200));
    assertEquals(0xC0123456, ColorUtil.multiplyColorAlpha(0xC0123456, 255));
  }

  @Test
  public void testGetOpacityFromColor() {
    assertEquals(PixelFormat.TRANSPARENT, ColorUtil.getOpacityFromColor(0x00000000));
    assertEquals(PixelFormat.TRANSPARENT, ColorUtil.getOpacityFromColor(0x00123456));
    assertEquals(PixelFormat.TRANSPARENT, ColorUtil.getOpacityFromColor(0x00FFFFFF));
    assertEquals(PixelFormat.TRANSLUCENT, ColorUtil.getOpacityFromColor(0xC0000000));
    assertEquals(PixelFormat.TRANSLUCENT, ColorUtil.getOpacityFromColor(0xC0123456));
    assertEquals(PixelFormat.TRANSLUCENT, ColorUtil.getOpacityFromColor(0xC0FFFFFF));
    assertEquals(PixelFormat.OPAQUE, ColorUtil.getOpacityFromColor(0xFF000000));
    assertEquals(PixelFormat.OPAQUE, ColorUtil.getOpacityFromColor(0xFF123456));
    assertEquals(PixelFormat.OPAQUE, ColorUtil.getOpacityFromColor(0xFFFFFFFF));
  }

  @Test
  public void testNormalize() {
    assertEquals(0x800B1621, ColorUtil.normalize(11, 22, 33, 0.5));
    assertEquals(0x00000000, ColorUtil.normalize(0, 0, 0, 0));
    assertEquals(0xFFFFFFFF, ColorUtil.normalize(255, 255, 255, 1));
    assertEquals(0xFF00FFFF, ColorUtil.normalize(-1, 256, 255, 1.1));
    assertEquals(0x000001FF, ColorUtil.normalize(0.4, 0.5, 255.4, -1));
  }
}

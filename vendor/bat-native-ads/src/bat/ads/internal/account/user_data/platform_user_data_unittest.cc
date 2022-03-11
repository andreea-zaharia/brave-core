/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/account/user_data/platform_user_data.h"

#include "bat/ads/internal/unittest_base.h"
#include "bat/ads/internal/unittest_util.h"

// npm run test -- brave_unit_tests --filter=BatAds*

namespace ads {

class BatAdsPlatformUserDataTest : public UnitTestBase {
 protected:
  BatAdsPlatformUserDataTest() = default;

  ~BatAdsPlatformUserDataTest() override = default;
};

TEST_F(BatAdsPlatformUserDataTest, GetPlatform) {
  // Arrange
  MockPlatformHelper(platform_helper_mock_, PlatformType::kMacOS);

  // Act
  const base::DictionaryValue platform = user_data::GetPlatform();

  // Assert
  base::DictionaryValue expected_platform;
  expected_platform.SetKey("platform", base::Value("macos"));

  EXPECT_EQ(expected_platform, platform);
}

}  // namespace ads

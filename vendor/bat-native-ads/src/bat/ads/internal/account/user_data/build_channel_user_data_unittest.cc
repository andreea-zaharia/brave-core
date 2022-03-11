/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/account/user_data/build_channel_user_data.h"

#include "base/values.h"
#include "bat/ads/internal/unittest_util.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- brave_unit_tests --filter=BatAds*

namespace ads {

TEST(BatAdsBuildChannelUserDataTest, GetBuildChannel) {
  // Arrange
  SetBuildChannel(false, "release");

  // Act
  const base::DictionaryValue build_channel = user_data::GetBuildChannel();

  // Assert
  base::DictionaryValue expected_build_channel;
  expected_build_channel.SetKey("buildChannel", base::Value("release"));

  EXPECT_EQ(expected_build_channel, build_channel);
}

}  // namespace ads

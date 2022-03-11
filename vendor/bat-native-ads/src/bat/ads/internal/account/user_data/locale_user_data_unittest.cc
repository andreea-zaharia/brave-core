/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/account/user_data/locale_user_data.h"

#include "bat/ads/internal/unittest_base.h"
#include "bat/ads/internal/unittest_util.h"

// npm run test -- brave_unit_tests --filter=BatAds*

namespace ads {

class BatAdsLocaleUserDataTest : public UnitTestBase {
 protected:
  BatAdsLocaleUserDataTest() = default;

  ~BatAdsLocaleUserDataTest() override = default;
};

TEST_F(BatAdsLocaleUserDataTest, GetLocaleForNonReleaseBuildChannel) {
  // Arrange
  SetBuildChannel(false, "beta");
  MockLocaleHelper(locale_helper_mock_, "en-GB");

  // Act
  const base::DictionaryValue locale = user_data::GetLocale();

  // Assert
  base::DictionaryValue expected_locale;

  EXPECT_EQ(expected_locale, locale);
}

TEST_F(BatAdsLocaleUserDataTest, GetLocaleForReleaseBuildChannel) {
  // Arrange
  SetBuildChannel(true, "release");
  MockLocaleHelper(locale_helper_mock_, "en-GB");

  // Act
  const base::DictionaryValue locale = user_data::GetLocale();

  // Assert
  base::DictionaryValue expected_locale;
  expected_locale.SetKey("countryCode", base::Value("GB"));

  EXPECT_EQ(expected_locale, locale);
}

TEST_F(BatAdsLocaleUserDataTest, GetLocaleForCountryNotInAnonymitySet) {
  // Arrange
  SetBuildChannel(true, "release");
  MockLocaleHelper(locale_helper_mock_, "en-MC");

  // Act
  const base::DictionaryValue locale = user_data::GetLocale();

  // Assert
  base::DictionaryValue expected_locale;

  EXPECT_EQ(expected_locale, locale);
}

TEST_F(BatAdsLocaleUserDataTest,
       GetLocaleForCountryNotInAnonymitySetButShouldClassifyAsOther) {
  // Arrange
  SetBuildChannel(true, "release");
  MockLocaleHelper(locale_helper_mock_, "en-CX");

  // Act
  const base::DictionaryValue locale = user_data::GetLocale();

  // Assert
  base::DictionaryValue expected_locale;
  expected_locale.SetKey("countryCode", base::Value("??"));

  EXPECT_EQ(expected_locale, locale);
}

}  // namespace ads

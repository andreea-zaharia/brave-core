/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ledger/internal/contributions/contribution_router.h"

#include <string>

#include "bat/ledger/internal/contributions/contribution_data.h"
#include "bat/ledger/internal/contributions/contribution_token_manager.h"
#include "bat/ledger/internal/contributions/external_contribution_processor.h"
#include "bat/ledger/internal/contributions/token_contribution_processor.h"
#include "bat/ledger/internal/external_wallet/external_wallet_manager.h"

namespace ledger {

const char ContributionRouter::kContextKey[] = "contribution-router";

Future<bool> ContributionRouter::SendContribution(
    ContributionType contribution_type,
    const std::string& publisher_id,
    double amount) {
  DCHECK(!publisher_id.empty());

  if (amount <= 0) {
    context().LogInfo(FROM_HERE) << "Attempting to send a contribution with "
                                    "zero amount";
    return MakeReadyFuture(true);
  }

  Contribution contribution{.type = contribution_type,
                            .publisher_id = publisher_id,
                            .amount = amount,
                            .source = GetCurrentSource()};

  switch (contribution.source) {
    case ContributionSource::kBraveVG:
    case ContributionSource::kBraveSKU:
      return context().Get<TokenContributionProcessor>().ProcessContribution(
          contribution);
    case ContributionSource::kExternal:
      return context().Get<ExternalContributionProcessor>().ProcessContribution(
          contribution);
  }
}

Future<absl::optional<double>> ContributionRouter::GetAvailableBalance() {
  if (context().Get<ExternalWalletManager>().HasExternalWallet()) {
    return context().Get<ExternalWalletManager>().GetBalance();
  }

  return context()
      .Get<ContributionTokenManager>()
      .GetAvailableTokenBalance(ContributionTokenType::kVG)
      .Then(base::BindOnce([](double d) { return absl::optional<double>(d); }));
}

ContributionSource ContributionRouter::GetCurrentSource() {
  return context().Get<ExternalWalletManager>().HasExternalWallet()
             ? ContributionSource::kExternal
             : ContributionSource::kBraveVG;
}

}  // namespace ledger

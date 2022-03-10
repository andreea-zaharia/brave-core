import * as React from 'react'
import { useSelector } from 'react-redux'

import { getLocale, splitStringForTag } from '../../../../common/locale'
import {
  BraveWallet,
  BuySendSwapViewTypes,
  ToOrFromType,
  SwapValidationErrorType,
  AmountPresetTypes,
  WalletState
} from '../../../constants/types'
import SwapInputComponent from '../swap-input-component'
import { SwapTooltip } from '../../desktop'

// Styled Components
import {
  StyledWrapper,
  ArrowDownIcon,
  ArrowButton,
  SwapNavButton,
  SwapButtonText,
  SwapButtonLoader,
  SwapDisclaimerText,
  SwapDisclaimerButton,
  SwapDisclaimerRow,
  AlertIcon,
  SwapFeesNoticeRow,
  SwapFeesNoticeText
} from './style'
import { LoaderIcon } from 'brave-ui/components/icons'
import { ResetButton } from '../shared-styles'
import { useSwap } from '../../../common/hooks'

export interface Props {
  isFetchingQuote: boolean
  validationError: SwapValidationErrorType | undefined
  onChangeSwapView: (view: BuySendSwapViewTypes, option?: ToOrFromType) => void
  onFilterAssetList: (asset?: BraveWallet.BlockchainToken) => void
}

function Swap (props: Props) {
  const {
    isFetchingQuote,
    validationError,
    onChangeSwapView,
    onFilterAssetList
  } = props

  // redux
  const {
    selectedNetwork
  } = useSelector((state: { wallet: WalletState }) => state.wallet)

  // internal hooks
  const {
    customSlippageTolerance,
    exchangeRate,
    flipSwapAssets,
    fromAmount,
    fromAsset,
    fromAssetBalance,
    onCustomSlippageToleranceChange,
    onSelectExpiration,
    onSelectSlippageTolerance,
    onSelectPresetAmount,
    onSubmitSwap,
    onSwapInputChange: onInputChange,
    onSwapQuoteRefresh,
    onToggleOrderType,
    orderExpiration,
    orderType,
    slippageTolerance,
    toAmount,
    toAsset,
    toAssetBalance,
    setSelectedPreset,
    selectedPreset,
    isSwapButtonDisabled
  } = useSwap()

  const onShowAssetTo = () => {
    onChangeSwapView('assets', 'to')
    onFilterAssetList(fromAsset)
  }

  const onShowAssetFrom = () => {
    onChangeSwapView('assets', 'from')
    onFilterAssetList(toAsset)
  }

  const submitText = React.useMemo(() => {
    if (validationError === 'insufficientBalance') {
      return getLocale('braveWalletSwapInsufficientBalance')
    }

    if (validationError === 'insufficientFundsForGas') {
      return getLocale('braveWalletSwapInsufficientFundsForGas')
    }

    if (validationError === 'insufficientAllowance' && fromAsset) {
      return getLocale('braveWalletSwapInsufficientAllowance')
        .replace('$1', fromAsset.symbol)
    }

    if (validationError === 'insufficientLiquidity') {
      return getLocale('braveWalletSwapInsufficientLiquidity')
    }

    if (validationError === 'unknownError') {
      return getLocale('braveWalletSwapUnknownError')
    }

    return getLocale('braveWalletSwap')
  }, [validationError, fromAsset])

  const disclaimerText = getLocale('braveWalletSwapDisclaimer')
  const { beforeTag, duringTag, afterTag } = splitStringForTag(disclaimerText)

  const onClick0x = () => {
    chrome.tabs.create({ url: 'https://0x.org' }, () => {
      if (chrome.runtime.lastError) {
        console.error('tabs.create failed: ' + chrome.runtime.lastError.message)
      }
    })
  }

  const onReset = () => {
    onInputChange('', 'from')
    onInputChange('', 'to')
    onInputChange('', 'rate')
    setPresetAmountValue(0)
  }

  const setPresetAmountValue = (percent: number) => {
    setSelectedPreset(percent as AmountPresetTypes)
    onSelectPresetAmount(percent)
  }

  const handleOnInputChange = (value: string, name: string) => {
    if (name === 'from' && selectedPreset) {
      // Clear preset
      setSelectedPreset(undefined)
    }
    onInputChange(value, name)
  }

  return (
    <StyledWrapper>
      <SwapInputComponent
        componentType='fromAmount'
        onSelectPresetAmount={setPresetAmountValue}
        onInputChange={handleOnInputChange}
        selectedAssetInputAmount={fromAmount}
        inputName='from'
        selectedAssetBalance={fromAssetBalance}
        selectedAsset={fromAsset}
        selectedNetwork={selectedNetwork}
        onShowSelection={onShowAssetFrom}
        validationError={validationError}
        autoFocus={true}
        selectedPreset={selectedPreset}
      />
      <ArrowButton onClick={flipSwapAssets}>
        <ArrowDownIcon />
      </ArrowButton>
      <SwapInputComponent
        componentType='toAmount'
        orderType={orderType}
        onInputChange={onInputChange}
        selectedAssetInputAmount={toAmount}
        inputName='to'
        selectedAssetBalance={toAssetBalance}
        selectedAsset={toAsset}
        selectedNetwork={selectedNetwork}
        onShowSelection={onShowAssetTo}
        validationError={validationError}
      />
      <SwapInputComponent
        componentType='exchange'
        orderType={orderType}
        onToggleOrderType={onToggleOrderType}
        onInputChange={onInputChange}
        selectedAssetInputAmount={exchangeRate}
        inputName='rate'
        selectedAsset={fromAsset}
        onRefresh={onSwapQuoteRefresh}
      />
      <SwapInputComponent
        componentType='selector'
        orderType={orderType}
        onSelectSlippageTolerance={onSelectSlippageTolerance}
        onSelectExpiration={onSelectExpiration}
        slippageTolerance={slippageTolerance}
        orderExpiration={orderExpiration}
        customSlippageTolerance={customSlippageTolerance}
        onCustomSlippageToleranceChange={onCustomSlippageToleranceChange}
      />
      <SwapNavButton
        disabled={isSwapButtonDisabled}
        buttonType='primary'
        onClick={onSubmitSwap}
      >
        {
          isFetchingQuote
            ? <SwapButtonLoader><LoaderIcon /></SwapButtonLoader>
            : <SwapButtonText>{submitText}</SwapButtonText>
        }
      </SwapNavButton>
      <ResetButton
        onClick={onReset}
        >
          {getLocale('braveWalletReset')}
      </ResetButton>
      <SwapFeesNoticeRow>
        <SwapFeesNoticeText>
          {getLocale('braveWalletSwapFeesNotice')}
        </SwapFeesNoticeText>
      </SwapFeesNoticeRow>
      <SwapDisclaimerRow>
        <SwapDisclaimerText>
          {beforeTag}
          <SwapDisclaimerButton onClick={onClick0x}>{duringTag}</SwapDisclaimerButton>
          {afterTag}
        </SwapDisclaimerText>
        <SwapTooltip
          text={getLocale('braveWalletSwapDisclaimerDescription')}
        >
          <AlertIcon />
        </SwapTooltip>
      </SwapDisclaimerRow>
    </StyledWrapper>
  )
}

export default Swap

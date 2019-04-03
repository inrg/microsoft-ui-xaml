﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "pch.h"
#include "common.h"

#include "TabView.g.h"
#include "TabView.properties.h"

class TabView :
    public ReferenceTracker<TabView, winrt::implementation::TabViewT>,
    public TabViewProperties
{

public:
    TabView();
    ~TabView() {}

    // IFrameworkElement
    void OnApplyTemplate();

    void OnPropertyChanged(const winrt::DependencyPropertyChangedEventArgs& args);

    void CloseTab(winrt::TabViewItem item);

    //IItemsControlOverrides
    void OnItemsChanged(winrt::IInspectable const& item);

private:
    void OnLoaded(const winrt::IInspectable& sender, const winrt::RoutedEventArgs& args);
    void OnSelectionChanged(const winrt::IInspectable& sender, const winrt::SelectionChangedEventArgs& args);
    void OnScrollDecreaseClick(const winrt::IInspectable& sender, const winrt::RoutedEventArgs& args);
    void OnScrollIncreaseClick(const winrt::IInspectable& sender, const winrt::RoutedEventArgs& args);
    void OnSizeChanged(const winrt::IInspectable& sender, const winrt::SizeChangedEventArgs& args);

    void UpdateTabWidths();

    tracker_ref<winrt::ContentPresenter> m_tabContentPresenter{ this };
    tracker_ref<winrt::FxScrollViewer> m_scrollViewer{ this };
    tracker_ref<winrt::RepeatButton> m_scrollDecreaseButton{ this };
    tracker_ref<winrt::RepeatButton> m_scrollIncreaseButton{ this };

    winrt::ListViewBase::Loaded_revoker m_loadedRevoker{};
    winrt::ListViewBase::SizeChanged_revoker m_sizeChangedRevoker{};
    winrt::ListViewBase::SelectionChanged_revoker m_selectionChangedRevoker{};

    winrt::RepeatButton::Click_revoker m_scrollDecreaseClickRevoker{};
    winrt::RepeatButton::Click_revoker m_scrollIncreaseClickRevoker{};
};
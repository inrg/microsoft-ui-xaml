﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "common.h"
#include "TreeViewNode.h"
#include "TreeView.h"
#include "TreeViewList.h"
#include "TreeViewItem.h"
#include "TreeViewItemAutomationPeer.h"
#include "ViewModel.h"
#include "TreeViewNode.h"
#include "RuntimeProfiler.h"
#include "InspectingDataSource.h"

static constexpr auto c_listControlName = L"ListControl"sv;

TreeView::TreeView()
{
    __RP_Marker_ClassById(RuntimeProfiler::ProfId_TreeView);

    SetDefaultStyleKey(this);

    m_rootNode.set(winrt::TreeViewNode());
    m_pendingSelectedNodes.set(winrt::make<Vector<winrt::TreeViewNode>>());
}

winrt::IVector<winrt::TreeViewNode> TreeView::RootNodes()
{
    auto x = m_rootNode.get().Children();
    return x;
}

TreeViewList* TreeView::ListControl()
{
    return winrt::get_self<TreeViewList>(m_listControl.get());
}

winrt::IInspectable TreeView::ItemFromContainer(winrt::DependencyObject const& container)
{
    return ListControl() ? ListControl()->ItemFromContainer(container) : nullptr;
}

winrt::DependencyObject TreeView::ContainerFromItem(winrt::IInspectable const& item)
{
    return ListControl() ? ListControl()->ContainerFromItem(item) : nullptr;
}

winrt::TreeViewNode TreeView::NodeFromContainer(winrt::DependencyObject const& container)
{
    return ListControl() ? ListControl()->NodeFromContainer(container) : nullptr;
}

winrt::DependencyObject TreeView::ContainerFromNode(winrt::TreeViewNode const& node)
{
    return ListControl() ? ListControl()->ContainerFromNode(node) : nullptr;
}

void TreeView::SelectedNode(winrt::TreeViewNode const& node)
{
    auto selectedNodes = SelectedNodes();
    if (selectedNodes.Size() > 0)
    {
        selectedNodes.Clear();
    }
    if (node)
    {
        selectedNodes.Append(node);
    }
}

winrt::TreeViewNode TreeView::SelectedNode()
{
    auto nodes = SelectedNodes();
    return nodes.Size() > 0 ? nodes.GetAt(0) : nullptr;
}


winrt::IVector<winrt::TreeViewNode> TreeView::SelectedNodes()
{
    if (auto listControl = ListControl())
    {
        if (auto vm = listControl->ListViewModel())
        {
            return vm->GetSelectedNodes();
        }
    }
    
    // we'll treat the pending selected nodes as SelectedNodes value if we don't have a list control or a view model
    return m_pendingSelectedNodes.get();
}

void TreeView::SelectedItem(winrt::IInspectable const& item)
{
    auto selectedItems = SelectedItems();
    if (selectedItems.Size() > 0)
    {
        selectedItems.Clear();
    }
    if (item)
    {
        selectedItems.Append(item);
    }
}

winrt::IInspectable TreeView::SelectedItem()
{
    auto items = SelectedItems();
    return items.Size() > 0 ? items.GetAt(0) : nullptr;
}

winrt::IVector<winrt::IInspectable> TreeView::SelectedItems()
{
    if (auto listControl = ListControl())
    {
        if (auto viewModel = listControl->ListViewModel())
        {
            return viewModel->GetSelectedItems();
        }
    }

    return nullptr;
}

void TreeView::Expand(winrt::TreeViewNode const& value)
{
    auto vm = ListControl()->ListViewModel();
    vm->ExpandNode(value);
}

void TreeView::Collapse(winrt::TreeViewNode const& value)
{
    auto vm = ListControl()->ListViewModel();
    vm->CollapseNode(value);
}

void TreeView::SelectAll()
{
    auto vm = ListControl()->ListViewModel();
    vm->SelectAll();
}


void TreeView::OnItemClick(const winrt::IInspectable& /*sender*/, const winrt::Windows::UI::Xaml::Controls::ItemClickEventArgs& args)
{
    auto itemInvokedArgs = winrt::make_self<TreeViewItemInvokedEventArgs>();
    itemInvokedArgs->InvokedItem(args.ClickedItem());
    if (SelectionMode() == winrt::TreeViewSelectionMode::Single)
    {
        auto clickedNode = ListControl()->NodeFromItem(args.ClickedItem());
        SelectedNode(clickedNode);
    }

    m_itemInvokedEventSource(*this, *itemInvokedArgs);
}

void TreeView::OnContainerContentChanging(const winrt::IInspectable& sender, const winrt::Windows::UI::Xaml::Controls::ContainerContentChangingEventArgs& args)
{
    m_containerContentChangedSource(sender.as<winrt::ListView>(), args);
}

void TreeView::OnNodeExpanding(const winrt::TreeViewNode& sender, const winrt::IInspectable& /*args*/)
{
    auto treeViewExpandingEventArgs = winrt::make_self<TreeViewExpandingEventArgs>();
    treeViewExpandingEventArgs->Node(sender);

    if (m_listControl)
    {
        if (auto expandingTVI = safe_try_cast<winrt::TreeViewItem>(ContainerFromNode(sender)))
        {
            //Update TVI properties
            if (!expandingTVI.IsExpanded())
            {
                expandingTVI.IsExpanded(true);
            }

            //Update TemplateSettings properties
            auto templateSettings = winrt::get_self<TreeViewItemTemplateSettings>(expandingTVI.TreeViewItemTemplateSettings());
            templateSettings->ExpandedGlyphVisibility(winrt::Visibility::Visible);
            templateSettings->CollapsedGlyphVisibility(winrt::Visibility::Collapsed);
        }
        m_expandingEventSource(*this, *treeViewExpandingEventArgs);
    }
}

void TreeView::OnNodeCollapsed(const winrt::TreeViewNode& sender, const winrt::IInspectable& /*args*/)
{
    auto treeViewCollapsedEventArgs = winrt::make_self<TreeViewCollapsedEventArgs>();
    treeViewCollapsedEventArgs->Node(sender);

    if (m_listControl)
    {
        if (auto collapsedTVI = safe_try_cast<winrt::TreeViewItem>(ContainerFromNode(sender)))
        {
            //Update TVI properties
            if (collapsedTVI.IsExpanded())
            {
                collapsedTVI.IsExpanded(false);
            }

            //Update TemplateSettings properties
            auto templateSettings = winrt::get_self<TreeViewItemTemplateSettings>(collapsedTVI.TreeViewItemTemplateSettings());
            templateSettings->ExpandedGlyphVisibility(winrt::Visibility::Collapsed);
            templateSettings->CollapsedGlyphVisibility(winrt::Visibility::Visible);
        }
        m_collapsedEventSource(*this, *treeViewCollapsedEventArgs);
    }
}

void TreeView::OnPropertyChanged(const winrt::DependencyPropertyChangedEventArgs& args)
{
    winrt::IDependencyProperty property = args.Property();

    if (property == s_SelectionModeProperty && m_listControl)
    {
        UpdateSelectionMode(SelectionMode());
    }
    else if (property == s_ItemsSourceProperty)
    {
        winrt::get_self<TreeViewNode>(m_rootNode.get())->IsContentMode(true);

        if (auto listControl = ListControl())
        {
            auto viewModel = listControl->ListViewModel();
            viewModel->IsContentMode(true);
        }

        winrt::get_self<TreeViewNode>(m_rootNode.get())->ItemsSource(ItemsSource());
    }
}

void TreeView::UpdateSelectionMode(winrt::TreeViewSelectionMode selectionMode)
{
    if (auto listControl = ListControl())
    {
        listControl->SelectionMode(winrt::ListViewSelectionMode::None);
        auto viewModel = listControl->ListViewModel();
        viewModel->SelectionMode(selectionMode);

        switch (selectionMode)
        {
        case winrt::TreeViewSelectionMode::None:
        case winrt::TreeViewSelectionMode::Single:
            UpdateItemsSelectionMode(false);
            break;

        case winrt::TreeViewSelectionMode::Multiple:
            UpdateItemsSelectionMode(true);
            break;
        }
    }
}

void TreeView::OnListControlDragItemsStarting(const winrt::IInspectable& sender, const winrt::DragItemsStartingEventArgs& args)
{
    auto treeViewArgs = winrt::make_self<TreeViewDragItemsStartingEventArgs>();
    treeViewArgs->DragItemsStartingEventArgs(args);
    m_dragItemsStartingEventSource(*this, *treeViewArgs);
}

void TreeView::OnListControlDragItemsCompleted(const winrt::IInspectable& sender, const winrt::DragItemsCompletedEventArgs& args)
{
    auto treeViewArgs = winrt::make_self<TreeViewDragItemsCompletedEventArgs>();
    treeViewArgs->DragItemsCompletedEventArgs(args);
    m_dragItemsCompletedEventSource(*this, *treeViewArgs);
}

void TreeView::UpdateItemsSelectionMode(bool isMultiSelect)
{
    auto listControl = ListControl();
    listControl->EnableMultiselect(isMultiSelect);

    auto viewModel = listControl->ListViewModel();
    int size = viewModel->Size();

    for (int i = 0; i < size; i++)
    {
        auto updateContainer = safe_cast<winrt::TreeViewItem>(listControl->ContainerFromIndex(i));
        if (updateContainer)
        {
            if (isMultiSelect)
            {
                if (auto targetNode = viewModel->GetNodeAt(i))
                {
                    if (viewModel->IsNodeSelected(targetNode))
                    {
                        winrt::VisualStateManager::GoToState(updateContainer, L"TreeViewMultiSelectEnabledSelected", false);
                    }
                    else
                    {
                        winrt::VisualStateManager::GoToState(updateContainer, L"TreeViewMultiSelectEnabledUnselected", false);
                    }
                }
            }
            else
            {
                winrt::VisualStateManager::GoToState(updateContainer, L"TreeViewMultiSelectDisabled", false);
            }
        }
    }
}

void TreeView::OnApplyTemplate()
{
    winrt::IControlProtected controlProtected = *this;
    m_listControl.set(GetTemplateChildT<winrt::TreeViewList>(c_listControlName, controlProtected));

    if (auto listControl = m_listControl.get())
    {
        auto listPtr = winrt::get_self<TreeViewList>(m_listControl.get());
        auto viewModel = listPtr->ListViewModel();
        if (!m_rootNode.get())
        {
            m_rootNode.set(winrt::TreeViewNode());
        }

        if (ItemsSource())
        {
            viewModel->IsContentMode(true);
        }
        viewModel->PrepareView(m_rootNode.get());
        viewModel->SetOwningList(listControl);
        viewModel->NodeExpanding({ this, &TreeView::OnNodeExpanding });
        viewModel->NodeCollapsed({ this, &TreeView::OnNodeCollapsed });

        UpdateSelectionMode(SelectionMode());

        m_itemClickRevoker = listControl.ItemClick(winrt::auto_revoke, { this, &TreeView::OnItemClick });
        m_containerContentChangingRevoker = listControl.ContainerContentChanging(winrt::auto_revoke, { this, &TreeView::OnContainerContentChanging });
        m_dragItemsStartingRevoker = listControl.DragItemsStarting(winrt::auto_revoke, { this, &TreeView::OnListControlDragItemsStarting });
        m_dragItemsCompletedRevoker = listControl.DragItemsCompleted(winrt::auto_revoke, { this, &TreeView::OnListControlDragItemsCompleted });

        if (m_pendingSelectedNodes && m_pendingSelectedNodes.get().Size() > 0)
        {
            auto selectedNodes = viewModel->GetSelectedNodes();
            for (auto const& node : m_pendingSelectedNodes.get())
            {
                selectedNodes.Append(node);
            }
            m_pendingSelectedNodes.get().Clear();
        }
    }
}

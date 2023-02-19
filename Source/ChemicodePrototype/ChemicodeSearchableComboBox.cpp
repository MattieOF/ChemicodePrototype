// Copyright Epic Games, Inc. All Rights Reservekd.

#include "ChemicodeSearchableComboBox.h"
#include "Framework/Application/SlateUser.h"

#define LOCTEXT_NAMESPACE "SearchableComboBox"

void SChemicodeSearchableComboBox::Construct(const FArguments& InArgs)
{
	check(InArgs._ComboBoxStyle);

	ItemStyle = InArgs._ItemStyle;
	MenuRowPadding = InArgs._ComboBoxStyle->MenuRowPadding;

	// Work out which values we should use based on whether we were given an override, or should use the style's version
	const FComboButtonStyle& OurComboButtonStyle = InArgs._ComboBoxStyle->ComboButtonStyle;
	const FButtonStyle* const OurButtonStyle = InArgs._ButtonStyle ? InArgs._ButtonStyle : &OurComboButtonStyle.ButtonStyle;

	this->OnComboBoxOpening = InArgs._OnComboBoxOpening;
	this->OnSelectionChanged = InArgs._OnSelectionChanged;
	this->OnGenerateWidget = InArgs._OnGenerateWidget;

	OptionsSource = InArgs._OptionsSource;
	CustomScrollbar = InArgs._CustomScrollbar;

	FilteredOptionsSource.Append(*OptionsSource);

	TAttribute<EVisibility> SearchVisibility = InArgs._SearchVisibility;
	const EVisibility CurrentSearchVisibility = SearchVisibility.Get();

	TSharedRef<SWidget> ComboBoxMenuContent =
		SNew(SBox)
		.MaxDesiredHeight(InArgs._MaxListHeight)
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SAssignNew(this->SearchField, SEditableTextBox)
				.HintText(LOCTEXT("Search", "Search"))
				.OnTextChanged(this, &SChemicodeSearchableComboBox::OnSearchTextChanged)
				.OnTextCommitted(this, &SChemicodeSearchableComboBox::OnSearchTextCommitted)
				.Visibility(SearchVisibility)
			]

			+ SVerticalBox::Slot()
			[
				SAssignNew(this->ComboListView, SComboListType)
				.ListItemsSource(&FilteredOptionsSource)
				.OnGenerateRow(this, &SChemicodeSearchableComboBox::GenerateMenuItemRow)
				.OnSelectionChanged(this, &SChemicodeSearchableComboBox::OnSelectionChanged_Internal)
				.SelectionMode(ESelectionMode::Single)
				.ExternalScrollbar(InArgs._CustomScrollbar)
			]
		];

	// Set up content
	TSharedPtr<SWidget> ButtonContent = InArgs._Content.Widget;
	if (InArgs._Content.Widget == SNullWidget::NullWidget)
	{
		SAssignNew(ButtonContent, STextBlock)
			.Text(NSLOCTEXT("SChemicodeSearchableComboBox", "ContentWarning", "No Content Provided"))
			.ColorAndOpacity(FLinearColor::Red);
	}


	SComboButton::Construct(SComboButton::FArguments()
		.ComboButtonStyle(&OurComboButtonStyle)
		.ButtonStyle(OurButtonStyle)
		.Method(InArgs._Method)
		.ButtonContent()
		[
			ButtonContent.ToSharedRef()
		]
		.MenuContent()
		[
			ComboBoxMenuContent
		]
		.HasDownArrow(InArgs._HasDownArrow)
		.ContentPadding(InArgs._ContentPadding)
		.ForegroundColor(InArgs._ForegroundColor)
		.OnMenuOpenChanged(this, &SChemicodeSearchableComboBox::OnMenuOpenChanged)
		.IsFocusable(true)
		);

	if (CurrentSearchVisibility == EVisibility::Visible)
	{
		SetMenuContentWidgetToFocus(SearchField);
	}
	else
	{
		SetMenuContentWidgetToFocus(ComboListView);
	}

	// Need to establish the selected item at point of construction so its available for querying
	// NB: If you need a selection to fire use SetItemSelection rather than setting an IntiallySelectedItem
	SelectedItem = InArgs._InitiallySelectedItem;
	if (TListTypeTraits<TSharedPtr<FString>>::IsPtrValid(SelectedItem))
	{
		ComboListView->Private_SetItemSelection(SelectedItem, true);
	}

}

void SChemicodeSearchableComboBox::ClearSelection()
{
	ComboListView->ClearSelection();
}

void SChemicodeSearchableComboBox::SetSelectedItem(TSharedPtr<FString> InSelectedItem)
{
	if (TListTypeTraits<TSharedPtr<FString>>::IsPtrValid(InSelectedItem))
	{
		ComboListView->SetSelection(InSelectedItem, ESelectInfo::OnNavigation);
	}
	else
	{
		ComboListView->SetSelection(SelectedItem, ESelectInfo::OnNavigation);
	}
}

TSharedPtr<FString> SChemicodeSearchableComboBox::GetSelectedItem()
{
	return SelectedItem;
}

void SChemicodeSearchableComboBox::RefreshOptions()
{
	// Need to refresh filtered list whenever options change
	FilteredOptionsSource.Reset();

	if (SearchText.IsEmpty())
	{
		FilteredOptionsSource.Append(*OptionsSource);
	}
	else
	{
		for (TSharedPtr<FString> Option : *OptionsSource)
		{
			if (Option->Find(SearchText.ToString(), ESearchCase::Type::IgnoreCase) >= 0)
			{
				FilteredOptionsSource.Add(Option);
			}
		}
	}

	ComboListView->RequestListRefresh();
}

void SChemicodeSearchableComboBox::SetOptionsSource(const TArray<TSharedPtr<FString>>* InOptionsSource)
{
	OptionsSource = InOptionsSource;

	FilteredOptionsSource.Empty();
	FilteredOptionsSource.Append(*OptionsSource);
}

TSharedRef<ITableRow> SChemicodeSearchableComboBox::GenerateMenuItemRow(TSharedPtr<FString> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (OnGenerateWidget.IsBound())
	{
		return SNew(SComboRow<TSharedPtr<FString>>, OwnerTable)
			.Style(ItemStyle)
			.Padding(MenuRowPadding)
			[
				OnGenerateWidget.Execute(InItem)
			];
	}
	else
	{
		return SNew(SComboRow<TSharedPtr<FString>>, OwnerTable)
			[
				SNew(STextBlock).Text(NSLOCTEXT("SlateCore", "ComboBoxMissingOnGenerateWidgetMethod", "Please provide a .OnGenerateWidget() handler."))
			];

	}
}

void SChemicodeSearchableComboBox::OnMenuOpenChanged(bool bOpen)
{
	if (bOpen == false)
	{
		if (TListTypeTraits<TSharedPtr<FString>>::IsPtrValid(SelectedItem))
		{
			// Ensure the ListView selection is set back to the last committed selection
			ComboListView->SetSelection(SelectedItem, ESelectInfo::OnNavigation);
		}

		// Set focus back to ComboBox for users focusing the ListView that just closed
		TSharedRef<SWidget> ThisRef = AsShared();
		FSlateApplication::Get().ForEachUser([&ThisRef](FSlateUser& User) {
			if (User.HasFocusedDescendants(ThisRef))
			{
				User.SetFocus(ThisRef);
			}
		});

	}
}


void SChemicodeSearchableComboBox::OnSelectionChanged_Internal(TSharedPtr<FString> ProposedSelection, ESelectInfo::Type SelectInfo)
{
	if (!ProposedSelection)
	{
		return;
	}

	// Ensure that the proposed selection is different from selected
	if (ProposedSelection != SelectedItem)
	{
		SelectedItem = ProposedSelection;
		OnSelectionChanged.ExecuteIfBound(ProposedSelection, SelectInfo);
	}

	// close combo as long as the selection wasn't from navigation
	if (SelectInfo != ESelectInfo::OnNavigation)
	{
		this->SetIsOpen(false);
	}
	else
	{
		ComboListView->RequestScrollIntoView(SelectedItem, 0);
	}
}

void SChemicodeSearchableComboBox::OnSearchTextChanged(const FText& ChangedText)
{
	SearchText = ChangedText;

	RefreshOptions();
}

void SChemicodeSearchableComboBox::OnSearchTextCommitted(const FText& InText, ETextCommit::Type InCommitType)
{
	if ((InCommitType == ETextCommit::Type::OnEnter) && FilteredOptionsSource.Num() > 0)
	{
		ComboListView->SetSelection(FilteredOptionsSource[0], ESelectInfo::OnKeyPress);
	}
}

FReply SChemicodeSearchableComboBox::OnButtonClicked()
{
	// if user clicked to close the combo menu
	if (this->IsOpen())
	{
		// Re-select first selected item, just in case it was selected by navigation previously
		TArray<TSharedPtr<FString>> SelectedItems = ComboListView->GetSelectedItems();
		if (SelectedItems.Num() > 0)
		{
			OnSelectionChanged_Internal(SelectedItems[0], ESelectInfo::Direct);
		}
	}
	else
	{
		SearchField->SetText(FText::GetEmpty());
		OnComboBoxOpening.ExecuteIfBound();
	}

	return SComboButton::OnButtonClicked();
}

#undef LOCTEXT_NAMESPACE

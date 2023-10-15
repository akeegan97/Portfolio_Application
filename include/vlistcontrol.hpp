#ifndef VLISTCONTROL_H
#define VLISTCONTROL_H
#include <wx/listctrl.h>
#include <wx/wx.h>
#include <numeric>
using namespace std;

template <typename T>
class VListControl : public wxListCtrl {
public:
    VListControl(wxWindow *parent, const wxWindowID id, const wxPoint &pos, const wxSize &size)
        : wxListCtrl(parent, id, pos, size, wxLC_REPORT | wxLC_VIRTUAL) {
        for (size_t i = 0; i < T::columnNames.size(); ++i) {
            this->AppendColumn(T::columnNames[i]);
            this->SetColumnWidth(i, T::columnWidths[i]);
        }
        this->Bind(wxEVT_LIST_COL_CLICK, [this](wxListEvent &evt){
            auto selectedListIndex = getFirstSelectedIndex();
            long selectedDataIndex;

            if(selectedDataIndex != -1){
                selectedDataIndex = this->orderedIndices[selectedListIndex];
                this->SetItemState(selectedListIndex, 0, wxLIST_STATE_SELECTED);
            }
            this->sortByColumn(evt.GetColumn());
            this->RefreshAfterUpdate();

            if(selectedListIndex != -1){
                auto listIndexToSelect = findIndexOfDataIndex(selectedDataIndex);
                this->SetItemState(listIndexToSelect, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
                this->EnsureVisible(listIndexToSelect);
            }
            this->sortAscending = !this->sortAscending;
        });
    }

    virtual wxString OnGetItemText(long index, long column) const wxOVERRIDE {
        const T& item = items[orderedIndices[index]];  
        return item.GetValue(column).GetString();      
    }

    long getFirstSelectedIndex() {
        return GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    }
    long findIndexOfDataIndex(long dataIndex) {
        return std::find(orderedIndices.begin(), orderedIndices.end(), dataIndex) - orderedIndices.begin();
    }
    void RefreshAfterUpdate() {
        this->SetItemCount(orderedIndices.size());
        this->Refresh();
    }
    // void setItems(std::vector<T> itemsToSet) {
    //     this->items = itemsToSet;
    //     this->orderedIndices = std::vector<long>(items.size());
    //     std::iota(orderedIndices.begin(),orderedIndices.end(),0);
    //     this->SetItemCount(items.size());
    // }
    template<typename U>
    void setItems(U&& itemsToSet) {
        this->items = std::forward<U>(itemsToSet);
        this->orderedIndices = std::vector<long>(items.size());
        std::iota(orderedIndices.begin(), orderedIndices.end(), 0);
        this->SetItemCount(items.size());
    }
    // void AddRow(const T& newItem){
    //     items.push_back(newItem);
    //     this->SetItemCount(items.size());
    //     this->Refresh();
    // }
    template<typename U>
    void AddRow(U&& newItem) {
        items.push_back(std::forward<U>(newItem));
        this->SetItemCount(items.size());
        this->Refresh();
    }
    void sortByColumn(int column) {
    std::sort(items.begin(), items.end(),
                [this, column](const T& a, const T& b) {
                  wxVariant aValue = a.GetValue(column);
                  wxVariant bValue = b.GetValue(column);

                    if (aValue.GetType() == "string" && bValue.GetType() == "string") {
                        return sortAscending ? aValue.GetString() < bValue.GetString() 
                                            : aValue.GetString() > bValue.GetString();
                    }else if (aValue.GetType() == "datetime" && bValue.GetType() == "datetime") {
                        return sortAscending ? aValue.GetDateTime() < bValue.GetDateTime() 
                                                    : aValue.GetDateTime() > bValue.GetDateTime();
                    }
                    else {  
                        double aNumeric = aValue.GetDouble();
                        double bNumeric = bValue.GetDouble();
                        return sortAscending ? aNumeric < bNumeric 
                                            : aNumeric > bNumeric;
                    }
                } 
            );
    }

    virtual int OnGetItemColumnImage(long item, long column) const wxOVERRIDE {
        return -1;
    }
    virtual wxListItemAttr *OnGetItemAttr(long item) const wxOVERRIDE {
        return nullptr;
    }

    std::vector<long> orderedIndices;
    std::vector<T> items;
    bool sortAscending = true;
};

#endif
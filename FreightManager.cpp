#include "FreightManager.h"
#include <fstream>
#include <sstream>
using namespace std;

static string trimLocal(const string& s) { return trim(s); }

static bool parseFreightRow(const string& line, Freight& out) {
    stringstream ss(line);
    string id, dest, timeStr;
    if (!getline(ss, id, ','))
        return false;
    if (!getline(ss, dest, ','))
        return false;
    if (!getline(ss, timeStr, ','))
        return false;

    id = trimLocal(id);
    dest = trimLocal(dest);
    timeStr = trimLocal(timeStr);
    if (id.empty() || dest.empty() || timeStr.empty())
        return false;
    
    int t = parseTimeToMinutes(timeStr);
    if (t < 0 || t >= 24 * 60)
        return false;
    out = Freight(id, dest, t);

    return true;
}

bool FreightManager::loadFromCSV(const string& path) {
    ifstream in(path);
    if (!in.is_open())
        return false;
    freights_.clear();
    string line;
    while (getline(in, line)) {
        string s = trimLocal(line);
        if (s.empty())
            continue;
        if (s.find("id") != string::npos && s.find("dest") != string::npos && s.find("time") != string::npos)
            continue;
        Freight f;
        if (parseFreightRow(s, f)) freights_.push_back(f);
    }
    return true;
}

int FreightManager::findIndexById(const string& id) const {
    for (size_t i = 0; i < freights_.size(); ++i)
        if (freights_[i].getId() == id)
            return static_cast<int>(i);

    return -1;
}

const Freight* FreightManager::findById(const string& id) const {
    int idx = findIndexById(id);
	return (idx >= 0) ? &freights_[static_cast<size_t>(idx)] : nullptr;
}

bool FreightManager::editById(const string& id, const string& newId, const string& newDest, int newTime) {
    int i = findIndexById(id);
    if (i < 0)
        return false;
    if (!newId.empty())
        freights_[i].setId(newId);
    if (!newDest.empty())
        freights_[i].setDest(newDest);
    if (newTime >= 0)
        freights_[i].setTime(newTime);
    return true;
}

bool FreightManager::deleteById(const string& id) {
    int i = findIndexById(id);
    if (i < 0)
        return false;
    freights_.erase(freights_.begin() + i);
    return true;
}

bool FreightManager::replaceById(const string& id, const Freight& replacement) {
    int i = findIndexById(id);
    if (i < 0)
        return false;
    freights_[static_cast<size_t>(i)] = replacement;
    return true;
}

void FreightManager::printList() const {
    cout << "Freights (" << freights_.size() << "):\n";
    for (const auto& f : freights_)
        cout << "  - " << f << "\n";
}


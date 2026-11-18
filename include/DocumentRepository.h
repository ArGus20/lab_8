#pragma once

#include "Models.h"
#include <tuple>
#include <vector>

class DocumentRepository {
public:
    int add(const PrintDocument& document);
    std::vector<PrintDocument> findAll();
    std::vector<PrintDocument> findPending();
    std::vector<PrintDocument> findTopPriority(int count);
    int getTotalPages();
    std::vector<std::tuple<std::string, std::string>> getDocumentsWithUsers();
};

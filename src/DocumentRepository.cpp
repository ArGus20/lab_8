#include "DocumentRepository.h"
#include "Database.h"

using namespace sqlite_orm;

int DocumentRepository::add(const PrintDocument& document) {
    return getStorage().insert(document);
}

std::vector<PrintDocument> DocumentRepository::findAll() {
    return getStorage().get_all<PrintDocument>();
}

std::vector<PrintDocument> DocumentRepository::findPending() {
    return getStorage().get_all<PrintDocument>(
        where(c(&PrintDocument::status) == "Pending")
    );
}

std::vector<PrintDocument> DocumentRepository::findTopPriority(int count) {
    return getStorage().get_all<PrintDocument>(
        order_by(&PrintDocument::priority).desc(),
        limit(count)
    );
}

int DocumentRepository::getTotalPages() {
    auto result = getStorage().sum(&PrintDocument::pageCount);
    return result ? *result : 0;
}

std::vector<std::tuple<std::string, std::string>>
DocumentRepository::getDocumentsWithUsers() {
    return getStorage().select(
        columns(&User::name, &PrintDocument::fileName),
        inner_join<PrintDocument>(
            on(c(&User::id) == &PrintDocument::userId)
        )
    );
}

#include "Database.h"
#include "UserRepository.h"
#include "DocumentRepository.h"
#include "SQLiteComparison.h"

#include <iostream>
#include <vector>
#include <exception>

void seedDatabase() {
    auto& db = getStorage();

    db.remove_all<PrintDocument>();
    db.remove_all<User>();

    std::vector<User> users = {
        {0, "Ivan Petrenko", "ivan@gmail.com", "IT"},
        {0, "Olena Koval", "olena@gmail.com", "HR"},
        {0, "Andrii Melnyk", "andrii@gmail.com", "Finance"},
        {0, "Maria Shevchenko", "maria@gmail.com", "Marketing"},
        {0, "Dmytro Bondar", "dmytro@gmail.com", "Administration"}
    };

    std::vector<int> ids;

    for (const auto& user : users) {
        ids.push_back(db.insert(user));
    }

    std::vector<PrintDocument> documents = {
        {0, ids[0], "report.pdf", 15, "Pending", 2},
        {0, ids[0], "project.docx", 8, "Printed", 1},
        {0, ids[0], "diagram.pdf", 12, "Pending", 3},
        {0, ids[1], "contract.pdf", 5, "Printing", 2},
        {0, ids[1], "application.docx", 10, "Pending", 3},
        {0, ids[1], "letter.pdf", 3, "Printed", 1},
        {0, ids[2], "budget.xlsx", 12, "Pending", 2},
        {0, ids[2], "analysis.pdf", 25, "Printing", 3},
        {0, ids[2], "invoice.pdf", 4, "Printed", 1},
        {0, ids[3], "presentation.pptx", 30, "Pending", 3},
        {0, ids[3], "banner.pdf", 6, "Pending", 2},
        {0, ids[3], "brochure.pdf", 16, "Printed", 1},
        {0, ids[4], "instruction.pdf", 20, "Printing", 2},
        {0, ids[4], "schedule.xlsx", 7, "Pending", 3},
        {0, ids[4], "document.docx", 9, "Printed", 1}
    };

    for (const auto& document : documents) {
        db.insert(document);
    }

    std::cout << "Test data created.\n";
}

void demonstrateUsers(IUserRepository& repo) {
    std::cout << "\nUSERS\n";

    for (const auto& user : repo.findAll()) {
        std::cout << user.id << " | "
                  << user.name << " | "
                  << user.email << " | "
                  << user.department << '\n';
    }

    User test{0, "Test User", "test@gmail.com", "Testing"};

    int id = repo.add(test);
    std::cout << "\nAdded user: " << id << '\n';

    auto found = repo.findById(id);

    if (found) {
        found->department = "Support";
        repo.update(*found);
        std::cout << "User updated.\n";
    }

    repo.remove(id);
    std::cout << "User deleted.\n";
}

void demonstrateDocuments() {
    DocumentRepository repo;

    std::cout << "\nPENDING DOCUMENTS\n";

    for (const auto& doc : repo.findPending()) {
        std::cout << doc.fileName << " | "
                  << doc.pageCount << " pages\n";
    }

    std::cout << "\nTOP 3 PRIORITY DOCUMENTS\n";

    for (const auto& doc : repo.findTopPriority(3)) {
        std::cout << doc.fileName << " | "
                  << doc.priority << '\n';
    }

    std::cout << "\nTOTAL PAGES: "
              << repo.getTotalPages() << '\n';

    std::cout << "\nUSERS AND DOCUMENTS\n";

    for (const auto& item : repo.getDocumentsWithUsers()) {
        std::cout << std::get<0>(item) << " | "
                  << std::get<1>(item) << '\n';
    }
}

void successfulTransaction() {
    auto& db = getStorage();

    try {
        bool result = db.transaction([&]() {
            User user{0, "Transaction User",
                      "transaction@gmail.com", "Printing"};

            int id = db.insert(user);

            PrintDocument document{
                0, id, "transaction.pdf", 10, "Pending", 3
            };

            db.insert(document);

            return true;
        });

        std::cout << "\nSuccessful transaction: "
                  << (result ? "COMMIT" : "ROLLBACK") << '\n';
    }
    catch (const std::exception& e) {
        std::cout << e.what() << '\n';
    }
}

void failedTransaction() {
    auto& db = getStorage();

    auto usersBefore = db.count<User>();
    auto documentsBefore = db.count<PrintDocument>();

    try {
        db.transaction([&]() {
            User user{0, "Rollback User",
                      "rollback@gmail.com", "Testing"};

            db.insert(user);

            PrintDocument document{
                0, 999999, "invalid.pdf", 5, "Pending", 2
            };

            db.insert(document);

            return true;
        });
    }
    catch (const std::exception& e) {
        std::cout << "\nExpected error: " << e.what() << '\n';
    }

    auto usersAfter = db.count<User>();
    auto documentsAfter = db.count<PrintDocument>();

    std::cout << "Users before: " << usersBefore
              << ", after: " << usersAfter << '\n';

    std::cout << "Documents before: " << documentsBefore
              << ", after: " << documentsAfter << '\n';

    if (usersBefore == usersAfter &&
        documentsBefore == documentsAfter) {
        std::cout << "ROLLBACK SUCCESSFUL\n";
    }
}

int main() {
    try {
        auto& db = getStorage();

        db.sync_schema();

        seedDatabase();

        UserRepository userRepository;

        demonstrateUsers(userRepository);
        demonstrateDocuments();

        successfulTransaction();
        failedTransaction();

        demonstrateSQLiteCAPI();

        std::cout << "\nProgram completed.\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}

#include <string>
#include <vector>
#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QObject>
#include <QListWidget>
#include <QString>
#include <QVector>

// Song class
class Song {
public:
    QString title;
    QString artist;
    QString duration;
    QString mediaPath;

    Song(const QString& t, const QString& a, const QString& d, const QString& m)
        : title(t), artist(a), duration(d), mediaPath(m) {}
};

// SongController class
class SongController : public QObject {
    Q_OBJECT

public:
    QVector<Song> allSongs;
    QVector<Song> playlistSongs;

    void addSong(const Song& song) {
        allSongs.push_back(song);
    }

    void removeSong(int index) {
        if (index >= 0 && index < allSongs.size()) {
            allSongs.remove(index);
        }
    }

    void addToPlaylist(int index) {
        if (index >= 0 && index < allSongs.size()) {
            playlistSongs.push_back(allSongs[index]);
        }
    }
};

QWidget* getLeftPanel(SongController* controller, QListWidget*& playlistWidget);
QWidget* getRightPanel(SongController* controller, QListWidget*& playlistWidget);
QWidget* getTextInputWithLabel(const char* name, QLineEdit*& input);
QWidget* getButtons(SongController* controller, QTableWidget* table, QLineEdit* titleInput, QLineEdit* artistInput, QLineEdit* durationInput, QLineEdit* pathInput);
QTableWidget* getTable();

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    auto* wnd = new QWidget{};
    auto* hLay = new QHBoxLayout{};

    auto* controller = new SongController{};
    QListWidget* playlistWidget = nullptr; // Initialize playlistWidget to nullptr

    hLay->addWidget(getLeftPanel(controller, playlistWidget));

    auto* moveButton = new QPushButton{ ">" };
    hLay->addWidget(moveButton);

    QObject::connect(moveButton, &QPushButton::clicked, [=]() {
        auto* table = qobject_cast<QTableWidget*>(wnd->findChild<QTableWidget*>("allSongsTable"));
        if (table) {
            int row = table->currentRow();
            if (row >= 0) {
                controller->addToPlaylist(row);
                const Song& song = controller->allSongs[row];
                playlistWidget->addItem(song.title + " - " + song.artist + " (" + song.duration + ")");
            }
        }
        });

    hLay->addWidget(getRightPanel(controller, playlistWidget));

    wnd->setLayout(hLay);
    wnd->setWindowTitle("Playlist Qt");
    wnd->show();

    return QApplication::exec();
}

QWidget* getRightPanel(SongController* controller, QListWidget*& playlistWidget) {
    auto* panel = new QWidget{};
    auto* vLay = new QVBoxLayout{};

    auto* label = new QLabel{ "Playlist" };
    vLay->addWidget(label);

    playlistWidget = new QListWidget{};
    vLay->addWidget(playlistWidget);

    auto* playButton = new QPushButton{ "Play" };
    vLay->addWidget(playButton);

    auto* nextButton = new QPushButton{ "Next" };
    vLay->addWidget(nextButton);

    panel->setLayout(vLay);
    return panel;
}

QWidget* getLeftPanel(SongController* controller, QListWidget*& playlistWidget) {
    auto* panel = new QWidget{};
    auto* vLayLeft = new QVBoxLayout{};

    auto* label = new QLabel{ "All songs" };
    vLayLeft->addWidget(label);

    auto* table = getTable();
    vLayLeft->addWidget(table);

    QLineEdit* titleInput = nullptr;
    QLineEdit* artistInput = nullptr;
    QLineEdit* durationInput = nullptr;
    QLineEdit* pathInput = nullptr;

    vLayLeft->addWidget(getTextInputWithLabel("Title:      ", titleInput));
    vLayLeft->addWidget(getTextInputWithLabel("Artist:     ", artistInput));
    vLayLeft->addWidget(getTextInputWithLabel("Duration:   ", durationInput));
    vLayLeft->addWidget(getTextInputWithLabel("Path:       ", pathInput));

    vLayLeft->addWidget(getButtons(controller, table, titleInput, artistInput, durationInput, pathInput));

    panel->setLayout(vLayLeft);
    return panel;
}

QTableWidget* getTable() {
    auto* table = new QTableWidget{};
    QStringList headers = { "Title", "Artist", "Duration", "Path" };
    table->setColumnCount(headers.count());
    table->setHorizontalHeaderLabels(headers);
    table->setObjectName("allSongsTable"); // set object name for identification
    return table;
}

QWidget* getButtons(SongController* controller, QTableWidget* table, QLineEdit* titleInput, QLineEdit* artistInput, QLineEdit* durationInput, QLineEdit* pathInput) {
    auto* panel = new QWidget{};
    auto* hLay = new QHBoxLayout{};

    auto* addButton = new QPushButton{ "Add" };
    QObject::connect(addButton, &QPushButton::clicked, [=]() {
        QString title = titleInput->text();
        QString artist = artistInput->text();
        QString duration = durationInput->text();
        QString path = pathInput->text();
        Song song(title, artist, duration, path);
        controller->addSong(song);
        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(title));
        table->setItem(row, 1, new QTableWidgetItem(artist));
        table->setItem(row, 2, new QTableWidgetItem(duration));
        table->setItem(row, 3, new QTableWidgetItem(path));
        });
    hLay->addWidget(addButton);

    auto* deleteButton = new QPushButton{ "Delete" };
    QObject::connect(deleteButton, &QPushButton::clicked, [=]() {
        int row = table->currentRow();
        controller->removeSong(row);
        table->removeRow(row);
        });
    hLay->addWidget(deleteButton);

    auto* updateButton = new QPushButton{ "Update" };
    hLay->addWidget(updateButton);

    auto* filterButton = new QPushButton{ "Filter" };
    hLay->addWidget(filterButton);

    panel->setLayout(hLay);
    return panel;
}

QWidget* getTextInputWithLabel(const char* name, QLineEdit*& input) {
    auto* panel = new QWidget{};
    auto* hLay = new QHBoxLayout{};

    auto* label = new QLabel{ name };
    hLay->addWidget(label);

    input = new QLineEdit{};
    hLay->addWidget(input);

    panel->setLayout(hLay);
    return panel;
}

#include "main.moc"

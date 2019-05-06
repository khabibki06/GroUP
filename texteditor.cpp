#include "texteditor.h"
#include "ui_texteditor.h"

TextEditor::TextEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TextEditor)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newDocument()));
    connect(ui->actionOpen,SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->actionSave,SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->actionSaveAs,SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(ui->actionPrint,SIGNAL(triggered()), this, SLOT(print()));
    connect(ui->actionExit,SIGNAL(triggered()), this, SLOT(exit()));
    connect(ui->actionCopy,SIGNAL(triggered()), this, SLOT(copy));
    connect(ui->actionCut,SIGNAL(triggered()), this, SLOT(cut()));
    connect(ui->actionPaste,SIGNAL(triggered()), this, SLOT(paste()));
    connect(ui->actionUndo,SIGNAL(triggered()), this, SLOT(undo()));
    connect(ui->actionRedo,SIGNAL(triggered()), this, SLOT(redo()));
    connect(ui->actionFont,SIGNAL(triggered()), this, SLOT(selectFont()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S),this,SLOT(save()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_O),this,SLOT(open()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q),this,SLOT(exit()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_N),this,SLOT(newDocument()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_P),this,SLOT(print()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_X),this,SLOT(cut()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_C),this,SLOT(copy()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_V),this,SLOT(paste()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z),this,SLOT(undo()));
//    connect(ui->actionBold,SIGNAL(triggered()), this, SLOT(setFontBold);
//    connect(ui->actionUnderline,SIGNAL(triggered()), this, SLOT(setFontUnderline);
//    connect(ui->actionItalic,SIGNAL(triggered()), this, SLOT(setFontItalic);
//    connect(ui->actionAbout,SIGNAL(triggered()), this, SLOT(about);


// Disable menu actions for unavailable features
#if !QT_CONFIG(printer)
    ui->actionPrint->setEnabled(false);
#endif

#if !QT_CONFIG(clipboard)
    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionPaste->setEnabled(false);
#endif
}

TextEditor::~TextEditor()
{
    delete ui;
}

void TextEditor::newDocument()
{
    currentFile.clear();
    ui->textEdit->setText(QString());
}

void TextEditor::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
}

void TextEditor::openFile(QString fileName)
{
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
}

void TextEditor::save()
{
    QString fileName;
    // If we don't have a filename from before, get one.
    if (currentFile.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this, "Save");
        currentFile = fileName;
    } else {
        fileName = currentFile;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}

void TextEditor::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    currentFile = fileName;
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}

void TextEditor::print()
{
#if QT_CONFIG(printer)
    QPrinter printDev;
#if QT_CONFIG(printdialog)
    QPrintDialog dialog(&printDev, this);
    if (dialog.exec() == QDialog::Rejected)
        return;
#endif // QT_CONFIG(printdialog)
    ui->textEdit->print(&printDev);
#endif // QT_CONFIG(printer)
}

void TextEditor::exit()
{
    QCoreApplication::quit();
}

void TextEditor::copy()
{
#if QT_CONFIG(clipboard)
    ui->textEdit->copy();
#endif
}

void TextEditor::cut()
{
#if QT_CONFIG(clipboard)
    ui->textEdit->cut();
#endif
}

void TextEditor::paste()
{
#if QT_CONFIG(clipboard)
    ui->textEdit->paste();
#endif
}

void TextEditor::undo()
{
     ui->textEdit->undo();
}

void TextEditor::redo()
{
    ui->textEdit->redo();
}

void TextEditor::selectFont()
{
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, this);
    if (fontSelected)
        ui->textEdit->setFont(font);
}

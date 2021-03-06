#include "HelpTool.h"

#include <QAction>
#include <QApplication>
#include <QMenu>
#include <QString>
#include <QToolBar>
#include <QWhatsThis>

#include "IException.h"
#include "MainWindow.h"
#include "Workspace.h"
#include "Preference.h"


namespace Isis {
  // Constructor
  HelpTool::HelpTool(QWidget *parent) : Tool(parent) {
    p_whatsThis = new QAction(parent);
    p_whatsThis->setShortcut(Qt::SHIFT + Qt::Key_F1);
    p_whatsThis->setText("&What's This?");
    p_whatsThis->setIcon(QPixmap(toolIconDir() + "/contexthelp.png"));
    p_whatsThis->setToolTip("What's This");
    QString whatsThis =
      "<b>Function:</b> Use this to get longer descriptions of button \
      functions \
      <p><b>Shortcut:</b> Shift+F1</p>";
    p_whatsThis->setWhatsThis(whatsThis);
    connect(p_whatsThis, SIGNAL(triggered()), this, SLOT(whatsThis()));

    p_aboutProgram = new QAction(parent);
    p_aboutProgram->setShortcut(Qt::CTRL + Qt::Key_H);
    QString s = "About ";
    s += QApplication::applicationName();
    p_aboutProgram->setText(s);
    connect(p_aboutProgram, SIGNAL(triggered()), this, SLOT(aboutProgram()));

  }

  // Add actions to the Help menu
  void HelpTool::addTo(QMenu *menu) {
    menu->addAction(p_whatsThis);
    menu->addAction(p_aboutProgram);
  }

  //! Add actions to the permanent toolbar
  void HelpTool::addToPermanent(QToolBar *perm) {
    perm->addAction(p_whatsThis);
  }

  // The user pressed the whatsThis button so do something
  void HelpTool::whatsThis() {
    QWhatsThis::enterWhatsThisMode();
  }


  /**
   * Slot to print program information
   *
   * @author  2007-06-12 Tracie Sucharski
   */
  void HelpTool::aboutProgram() {
    FileName file(
                  "$ISISROOT/doc/Application/presentation/PrinterFriendly/" +
                  QApplication::applicationName() + "/" +
                  QApplication::applicationName() + ".html");

    PvlGroup &uig = Preference::Preferences().findGroup("UserInterface");
    QString command = (QString) uig["GuiHelpBrowser"] +
                      (QString)" file:" + file.expanded() + " &";
    if(system(command.toLatin1().data()) != 0) {
      IString msg = "Failed to execute [" + command + "]";
      throw IException(IException::Programmer, msg, _FILEINFO_);
    }
  }
}

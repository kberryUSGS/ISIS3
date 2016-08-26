#ifndef ControlNetEditor_h
#define ControlNetEditor_h

#include "Tool.h"
#include "ControlPoint.h"

#include "Latitude.h"
#include "Longitude.h"

#include <QCloseEvent>
#include <QPalette>
#include <QPointer>
#include <QStringList>

class QObject;
class QWidget;

namespace Isis {
  class ControlNet;
  class ControlPoint;
  class SerialNumberList;

  /**
   * @brief Control Network editor base class
   *
   * @ingroup Visualization Tools
   *
   * @author ????-??-?? Tracie Sucharski
   *
   * @internal
   *   @history 2008-11-24 Jeannie Walldren - Changed name from TieTool.
   *                          Replace references to PointEdit class with
   *                          ControlPointEdit. Added "Goodness of Fit" to right
   *                          and left measure info.
   *   @history 2008-11-26  Jeannie Walldren - Added "Number of Measures" to
   *                          QnetTool point information. Defined updateNet() so
   *                          that the QnetTool window's title bar contains the
   *                          name of the control net file. Created
   *                          ignoreChanged() signal, modified pointSaved() and
   *                          createQnetTool() so message box appears if users are
   *                          saving an "Ignore" point and asks whether they would
   *                          like to set Ignore=false.
   *   @history 2008-12-09 Tracie Sucharski - Cleaned up some
   *                          signal/slot connections between QnetTool and
   *                          QnetNavTool for deleting or adding ControlPoints.
   *   @history 2008-12-09 Tracie Sucharski - Add new public slot
   *                          refresh to handle the ignorePoints and deletePoints
   *                          from the QnetNavTool.
   *   @history 2008-12-10 Jeannie Walldren - Added slot methods
   *                          viewTemplateFile() and setTemplateFile() to allow
   *                          user to view, edit or choose a new template file.
   *                          Added "What's this?" descriptions to actions.
   *   @history 2008-12-15 Jeannie Walldren - Some QMessageBox
   *                          warnings had strings tacked on to the list of
   *                          errors.  These strings were changed to iExceptions
   *                          and added to the error stack to conform with Isis
   *                          standards.
   *   @history 2008-12-15 Jeannie Walldren - Created newHoldPoint() method.
   *                          Replaced references to QnetGroundPointDialog with
   *                          QnetHoldPointDialog. Disabled ground point check box
   *                          so user may see whether the point is ground but may
   *                          not change this.  Thus setGroundPoint() and
   *                          newGroundPoint() methods still exist but are not
   *                          currently called.
   *   @history 2008-12-30 Jeannie Walldren - Modified to set measures in
   *                          viewports to Ignore=False if when saving, the user
   *                          chooses to set a point's Ignore=False. Replaced
   *                          references to ignoreChanged() with
   *                          ignorePointChanged(). Added signals
   *                          ignoreLeftChanged() and ignoreRightChanged().
   *   @history 2008-12-31 Jeannie Walldren - Added question box to pointSaved()
   *                          method to ask user whether the reference measure
   *                          should be replaced with the measure in the left
   *                          viewport. Added documentation.
   *   @history 2009-03-09 Jeannie Walldren - Modified createPoint() method to
   *                          clear the error stack after displaying a QMessageBox
   *                          to the user
   *   @history 2009-03-17 Tracie Sucharski - Added the ability to save the
   *                          registration chips to the Options menu.
   *   @history 2010-01-27 Jeannie Walldren - Fixed bug in setIgnoreLeftMeasure()
   *                          and setIgnoreRightMeasure() that caused segmentation
   *                          faults. Added question box to warn user that they
   *                          are saving changes to an ignored measure.
   *   @history 2010-06-02 Jeannie Walldren - Added cancelHoldPoint() method and
   *                          connected this slot to QnetHoldPointDialog's
   *                          holdCancelled() signal.
   *   @history 2010-06-03 Jeannie Walldren - Initialized pointers to null in
   *                          constructor.
   *   @history 2010-06-08 Jeannie Walldren - Fixed bug in drawAllMeasurments() so
   *                          that points with ignored measures matching the image
   *                          shown are drawn as yellow unless the QnetTool is
   *                          open to this point.  Changed QMessageBox types for
   *                          consistency with other qnet warnings and errors
   *   @history 2010-07-01 Jeannie Walldren - Added createToolBarWidget(),
   *                          showNavWindow() methods to reopen NavTool
   *                          (if closed) whenever the Tie tool button on the tool
   *                          pad is clicked.  Modified drawAllMeasurments() to
   *                          draw points selected in QnetTool last so they lay on
   *                          top of all other points in the image.  Replaced
   *                          #includes with forward class declarations and moved
   *                          #include to .cpp file.
   *   @history 2010-07-12 Jeannie Walldren - Fixed bug in deletePoint() method.
   *                          To prevent a seg fault, set m_controlPoint to NULL
   *                          and emit editPointChanged with an empty string if
   *                          the entire point is deleted.
   *   @history 2010-07-12 Jeannie Walldren - Fixed bug in newHoldPoint() method
   *                          that was causing the Hold Point Dialog to call the
   *                          reject() command.  Updated documentation.
   *  @history 2010-10-28 Tracie Sucharski - Fixed some include problems caused
   *                          by changes made to the ControlNet,ControlPoint,
   *                          ControlMeasure header files.  Remove findPointFiles
   *                          method, the code is now in the createPoint method.
   *  @history 2010-11-19 Tracie Sucharski - Renamed pointSaved slot to measureSaved.
   *  
   *
   *   @history 2010-11-17 Eric Hyer - Added newControlNetwork SIGNAL
   *   @history 2010-11-22 Eric Hyer - Added stretchChipViewport SIGNAL for forwarding of SIGNAL
   *                           from StretchTool to ChipViewport
   *   @history 2010-12-08 Eric Hyer - Template filename now shown.  Widgets
   *                           in main window now organized into groupBoxes.  Removed
   *                           Options menu and moved registration menu to main menu bar.
   *                           Added toolbar for actions also in menu.  All actions now
   *                           have icons.
   *   @history 2010-12-14 Eric Hyer - Template editor is now a widget within
   *                           the main window.  Newly saved template files take effect
   *                           after saving.
   *   @history 2010-12-17 Eric Hyer - Fixed bug where current template file
   *                           was not being updated with saveAs.  Moved template file
   *                           display to outside of control point groupbox.
   *   @history 2011-06-08 Tracie Sucharski - Point types renamed:
   *                           Ground ----> Fixed
   *                           Tie    ----> Free
   *   @history 2011-06-28 Tracie Sucharski - Added methods, "loadMeasureTable"
   *                          and "measureColumnToString".  TODO:  If these
   *                          stay in qnet, they really need cleaning up.  This
   *                          was a quick & dirty band-aid until cneteditor is
   *                          ready to be folded into qnet.
   *   @history 2011-07-27 Tracie Sucharski - Added method to return the radius
   *                          from a Dem if one is open.
   *   @history 2011-09-16 Tracie Sucharski - Added method to draw Fixed and
   *                          Constrained points on the ground source viewport.
   *   @history 2012-01-11 Tracie Sucharski - Add error check for invalid lat, 
   *                          lon when creating new control point.
   *   @history 2012-04-09 Tracie Sucharski - When checking if left measure
   *                          editLock has changed, use measure->IsEditLocked()
   *                          instead of this classes IsMeasureLocked().
   *   @history 2012-04-16 Tracie Sucharski - When attempting to un-lock a measure
   *                          print error if point is locked.
   *   @history 2012-04-26 Tracie Sucharski - Cleaned up private slot,measureSaved.
   *                          Abstracted out checking for a new reference measure
   *                          and updating the surface point for a ground point.
   *   @history 2012-05-07 Tracie Sucharski - Removed code in measureSaved to re-load
   *                          left measure if left and right are the same, this is
   *                          already handled in ControlPointEdit::saveMeasure.
   *   @history 2012-05-08 Tracie Sucharski - Fixed bug where m_leftFile was not
   *                          being reset in mouseButtonRelease.  Change m_leftFile
   *                          from a std::string to a QString.
   *   @history 2012-06-12 Tracie Sucharski - Change made to measureSaved on 2012-04-26 caused a
   *                          bug where if no ground is loaded the checkReference was not being
   *                          called and reference measure could not be changed and there was no
   *                          warning printed.  Fix:  Only call checkReference if there is no
   *                          explicit reference for the point, otherwise simply set reference to
   *                          left measure.  If ground source is on the left do not print warning
   *                          about new reference measure.
   *   @history 2012-07-30 Tracie Sucharski - Fixed bug, program would seg fault when left-clicking
   *                          on a cube when there were no control points.  Before calling
   *                          FindClosest, make sure the control net has points.
   *   @history 2012-09-28 Tracie Sucharski - In ::loadPoint when looking for ground source in
   *                          the right combo box, use the fileName, not the serial number.  The
   *                          ground source serial number will not be the fileName if the
   *                          Instrument group is retained in the labels.  Fixes #1018
   * @history 2012-10-04 Tracie Sucharski - If the ground source serial number already exists in 
   *                          the serial number list, print error and clear out ground information.
   *                          Fixes #1018
   * @history 2013-05-09 Tracie Sucharski - Check for user selecting all measures for deletion and 
   *                          print warning that point will be deleted. Fixes #1491.
   * @history 2013-05-09 Tracie Sucharski - For editing (left button) and deleting (right button), 
   *                          Swapped checking for empty network and not allowing mouse clicks on
   *                          the ground source. First check if there are any points in the network.
   *                          If not print message and return.  Fixes #1493.
   * @history 2013-05-16 Tracie Sucharski - Fixed some bugs when closing a ground source, opening 
   *                          a new ground source, and printing errors when point does not exist
   *                          on current ground source.  Fixes #1655.
   * @history 2013-11-07 Tracie Sucharski - Moved error checking on edit locked measures from
   *                          ::measureSaved to ControlPointEdit::saveMeasure.  The error checking now
   *                          forces the edit lock check box to be unchecked before the measure
   *                          can be saved.  Fixes #1624.
   * @history 2013-12-05 Tracie Sucharski - Added error check for an ignored reference measure 
   *                          when changing point type to constrained or fixed or when saving
   *                          constrained or fixed point with an ignored reference measure.
   *                          Added new method, loadGroundMeasure, so that loadPoint does not
   *                          need to be called from setPointType.  This avoids reloading the point
   *                          in case the ignored flag has been changed, but measure has not been
   *                          saved.  References #1603.
   *  
   *  
   *  
   * @history 2014-07-18 Kimberly Oyama and Tracie Sucharski - Refactored from QnetTools
   */
  class ControlNetEditor : public QObject {
    Q_OBJECT

    public:
      ControlNetEditor (SerialNumberList *serialNumberList, ControlNet *cnet, QWidget *parent);
      virtual ~ControlNetEditor ();

      ControlNet *controlNet();
      const ControlNet *controlNet() const;

      SerialNumberList *serialNumberList();
      const SerialNumberList *serialNumberList() const;

      
    signals:
      void controlPointCreated(ControlPoint *controlPoint);
      void netChanged();
      void newControlNetwork(ControlNet *);
      void newControlPointCreated(ControlPoint *);
      void controlPointDeleted();

    public slots:
      void createPoint(QString serialNumber, double sample, double line);
      void createPoint(QString serialNumber, Latitude lat, Longitude lon);
      //void createFixedPoint(double lat,double lon);
      //void modifyPoint(ControlPoint *point);
      void deleteControlPoint(QString pointId);//make this take string id

    private slots:
//    void checkReference();
//    void updateGroundPosition();
//    void savePoint();

    private:
      QStringList findPointFiles(double lat, double lon);


      QWidget *m_parent;
      QPointer<ControlPoint> m_editPoint;

      QStringList m_pointFiles;

      QString m_lastUsedPointId;
      QPointer<ControlNet> m_controlNet;
      SerialNumberList *m_serialNumberList;
  };
};

#endif

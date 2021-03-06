/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

#ifndef __qSlicerMarkupsModuleWidget_h
#define __qSlicerMarkupsModuleWidget_h

// SlicerQt includes
#include "qSlicerAbstractModuleWidget.h"

#include "qSlicerMarkupsModuleExport.h"

class qSlicerMarkupsModuleWidgetPrivate;
class vtkMRMLNode;
class QTableWidgetItem;
class vtkSlicerMarkupsLogic;
class QModelIndex;
class QShortcut;

/// \ingroup Slicer_QtModules_Markups
class Q_SLICER_QTMODULES_MARKUPS_EXPORT qSlicerMarkupsModuleWidget :
  public qSlicerAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qSlicerAbstractModuleWidget Superclass;
  qSlicerMarkupsModuleWidget(QWidget *parent=0);
  virtual ~qSlicerMarkupsModuleWidget();

  /// Set up the GUI from mrml when entering
  virtual void enter();
  /// Disconnect from scene when exiting
  virtual void exit();

  /// Manage short cuts that allow key bindings for certain functions
  void installShortcuts();
  void removeShortcuts();

  /// Get the logic in the proper class
  vtkSlicerMarkupsLogic *markupsLogic();

  /// Refresh the gui from the currently active markup node as determined by
  /// the selection node
  /// \sa updateWidgetFromDisplayNode()
  void updateWidgetFromMRML();
  /// Update the GUI elements related to the display properties from MRML, by
  /// getting the display node associated with the active markups node.
  /// \sa  updateWidgetFromMRML()
  void updateWidgetFromDisplayNode();
  /// Refresh a row of the gui from the mth markup in the currently active
  /// markup node as defined by the selection node combo box
  void updateRow(int m);

  /// Add observations to the markups node, and remove them from other markups
  /// nodes (from all nodes if markupsNode is null)
  void observeMarkupsNode(vtkMRMLNode *markupsNode);

  /// Reset the GUI elements: clear out the table
  void clearGUI();

  /// Set up the logic default display settings from the appplication settings
  void updateLogicFromSettings();

public slots:

  /// Respond to the scene events
  /// when a markups node is added, make it the active one in the combo box
  void onNodeAddedEvent(vtkObject* scene, vtkObject* node);
  /// When a node is removed and it is the last one in the scene, clear out
  /// the gui - the node combo box will signal that a remaining node has been
  /// selected and the GUI will update separately in that case
  void onNodeRemovedEvent(vtkObject* scene, vtkObject* node);
  /// Update the table after a scene is imported
  void onMRMLSceneEndImportEvent();
  /// Update the table after batch processing is done, needed to trigger
  /// an update after a markups file is read in
  void onMRMLSceneEndBatchProcessEvent();
  /// Clear out the gui when the scene is closed
  void onMRMLSceneEndCloseEvent();
  /// Respond to the p key being pressed
  void onPKeyActivated();

  /// Called from enter and on import/add mrml events to ask if the user
  /// wishes to convert annotation fiducial heirarchies into markups list nodes.
  /// \sa enter(), onMRMLSceneEndImportEvent(), onMRMLSceneEndBatchProcessEvent()
  void checkForAnnotationFiducialConversion();
  /// Uses the Logic to do the conversion from annotation fiducials, moving
  /// them from hierarchies to Markups list nodes
  void convertAnnotationFiducialsToMarkups();

  /// Display property slots
  void onSelectedColorPickerButtonChanged(QColor qcolor);
  void onUnselectedColorPickerButtonChanged(QColor qcolor);
  void onGlyphTypeComboBoxChanged(QString value);
  void onGlyphScaleSliderWidgetChanged(double value);
  void onTextScaleSliderWidgetChanged(double value);
  void onOpacitySliderWidgetChanged(double value);

  void onMarkupScaleSliderWidgetValueChanged(double value);

  /// Display property button slots
  void onResetToDefaultDisplayPropertiesPushButtonClicked();
  void onSaveToDefaultDisplayPropertiesPushButtonClicked();

  /// List button slots
  void onVisibilityOnAllMarkupsInListPushButtonClicked();
  void onVisibilityOffAllMarkupsInListPushButtonClicked();
  void onVisibilityAllMarkupsInListToggled();
  void onLockAllMarkupsInListPushButtonClicked();
  void onUnlockAllMarkupsInListPushButtonClicked();
  void onLockAllMarkupsInListToggled();
  void onSelectAllMarkupsInListPushButtonClicked();
  void onDeselectAllMarkupsInListPushButtonClicked();
  void onSelectedAllMarkupsInListToggled();
  void onAddMarkupPushButtonClicked();
  void onMoveMarkupUpPushButtonClicked();
  void onMoveMarkupDownPushButtonClicked();
  void onDeleteMarkupPushButtonClicked();
  void onDeleteAllMarkupsInListPushButtonClicked();

  /// Update the selection node from the combo box
  void onActiveMarkupMRMLNodeChanged(vtkMRMLNode *markupsNode);
  /// Update the combo box from the selection node
  void onSelectionNodeActivePlaceNodeIDChanged();

  /// When the user clicks in the comob box to create a new markups node,
  /// make sure that a display node is added
  void onActiveMarkupMRMLNodeAdded(vtkMRMLNode *markupsNode);

  /// Toggle the markups node visibility flag
  void onListVisibileInvisiblePushButtonClicked();
  /// Update the icon and tool tip on the list visibility button
  void updateListVisibileInvisiblePushButton(int visibleFlag);

  /// Toggle the markups node locked flag
  void onListLockedUnlockedPushButtonClicked();

  /// Update the markup label from the line edit entry
  void onNameFormatLineEditTextEdited(const QString text);

  /// Reset the name format string and use list name to default
  void onResetNameFormatToDefaultPushButtonClicked();

  /// Iterate over the markups in the currently active list and rename them
  /// using the current name format. Attempts to preserve any numbers already
  /// present.
  void onRenameAllWithCurrentNameFormatPushButtonClicked();

  /// Update the mrml node from the table
  void onActiveMarkupTableCellChanged(int row, int column);
  /// React to clicks in the table
  void onActiveMarkupTableCellClicked(QTableWidgetItem* item);
  /// React to arrows setting the current cell
  void onActiveMarkupTableCurrentCellChanged(int currentRow, int currentColumn,
                                             int previousRow, int previousColumn);
  /// Provide a right click menu in the table
  void onRightClickActiveMarkupTableWidget(QPoint pos);
  /// Jump slices action slot
  void onJumpSlicesActionTriggered();
  /// Right click action slots
  void onMoveToOtherListActionTriggered(QString destinationPosition);
  /// Respond to the user selecting another list to move selected markups to
  /// the named list, calls logic method to do the move if it can find both
  /// markups nodes in the scene
  void moveSelectedToNamedList(QString listName);

  /// Enable/disable editing the table if the markups node is un/locked
  void onActiveMarkupsNodeLockModifiedEvent();
  /// Update the format text entry from the node
  void onActiveMarkupsNodeLabelFormatModifiedEvent();
  /// Update the table with the modified point information if the node is
  /// active
  void onActiveMarkupsNodePointModifiedEvent(vtkObject *caller, vtkObject *callData);
  /// Update the table with the new markup if the node is active
  void onActiveMarkupsNodeMarkupAddedEvent();//vtkMRMLNode *markupsNode);
  /// Update the table for the removed markup if the node is active
  void onActiveMarkupsNodeMarkupRemovedEvent();//vtkMRMLNode *markupsNode);
  /// Update a table row from a modified markup
  void onActiveMarkupsNodeNthMarkupModifiedEvent(vtkObject *caller, vtkObject *callData);
  /// Update the display properties widgets when the display node is modified
  void onActiveMarkupsNodeDisplayModifiedEvent();

  /// Create a new markups node and copy the display node settings from the
  /// current markups node if set, otherwise just uses the defaults.
  void onNewMarkupWithCurrentDisplayPropertiesTriggered();

protected:
  QScopedPointer<qSlicerMarkupsModuleWidgetPrivate> d_ptr;

  virtual void setup();

private:
  Q_DECLARE_PRIVATE(qSlicerMarkupsModuleWidget);
  Q_DISABLE_COPY(qSlicerMarkupsModuleWidget);

  QShortcut *pToAddShortcut;
};

#endif

/*=auto=========================================================================

Portions (c) Copyright 2005 Brigham and Women\"s Hospital (BWH) All Rights Reserved.

See COPYRIGHT.txt
or http://www.slicer.org/copyright/copyright.txt for details.

Program:   3D Slicer
Module:    $RCSfile: vtkMRMLVolumeDisplayNode.cxx,v $
Date:      $Date: 2006/03/17 15:10:10 $
Version:   $Revision: 1.2 $

=========================================================================auto=*/

// MRML includes
#include "vtkMRMLVolumeDisplayNode.h"
#include "vtkMRMLVolumeNode.h"

// VTK includes
#include <vtkCommand.h>
#include <vtkImageData.h>

// Initialize static member that controls resampling -- 
// old comment: "This offset will be changed to 0.5 from 0.0 per 2/8/2002 Slicer 
// development meeting, to move ijk coordinates to voxel centers."

//----------------------------------------------------------------------------
vtkMRMLVolumeDisplayNode::vtkMRMLVolumeDisplayNode()
{
  // try setting a default greyscale color map
  //this->SetDefaultColorMap(0);
}

//----------------------------------------------------------------------------
vtkMRMLVolumeDisplayNode::~vtkMRMLVolumeDisplayNode()
{
}

//----------------------------------------------------------------------------
void vtkMRMLVolumeDisplayNode::WriteXML(ostream& of, int nIndent)
{
  Superclass::WriteXML(of, nIndent);
}

//----------------------------------------------------------------------------
void vtkMRMLVolumeDisplayNode::ReadXMLAttributes(const char** atts)
{

  Superclass::ReadXMLAttributes(atts);
}

//----------------------------------------------------------------------------
// Copy the node\"s attributes to this object.
// Does NOT copy: ID, FilePrefix, Name, VolumeID
void vtkMRMLVolumeDisplayNode::Copy(vtkMRMLNode *anode)
{
  Superclass::Copy(anode);
  //vtkMRMLVolumeDisplayNode *node = (vtkMRMLVolumeDisplayNode *) anode;

}

//---------------------------------------------------------------------------
void vtkMRMLVolumeDisplayNode::ProcessMRMLEvents(vtkObject *caller,
                                                 unsigned long event,
                                                 void *callData)
{
  this->Superclass::ProcessMRMLEvents(caller, event, callData);
  if (event ==  vtkCommand::ModifiedEvent)
    {
    this->UpdateImageDataPipeline();
    }
}

//----------------------------------------------------------------------------
void vtkMRMLVolumeDisplayNode::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

//-----------------------------------------------------------
void vtkMRMLVolumeDisplayNode::UpdateScene(vtkMRMLScene *scene)
{
  this->Superclass::UpdateScene(scene);
}

//-----------------------------------------------------------
void vtkMRMLVolumeDisplayNode::UpdateReferences()
{
  this->Superclass::UpdateReferences();
}

//---------------------------------------------------------------------------
vtkImageData* vtkMRMLVolumeDisplayNode::GetImageData()
{
  if (!this->GetInputImageData())
    {
    return 0;
    }
  this->UpdateImageDataPipeline();
  return this->GetOutputImageData();
}

//----------------------------------------------------------------------------
void vtkMRMLVolumeDisplayNode
::SetInputImageData(vtkImageData *imageData)
{
  this->SetInputToImageDataPipeline(imageData);
  this->Modified();
}

//----------------------------------------------------------------------------
void vtkMRMLVolumeDisplayNode::SetInputToImageDataPipeline(vtkImageData *vtkNotUsed(imageData))
{
}

//----------------------------------------------------------------------------
vtkImageData* vtkMRMLVolumeDisplayNode::GetInputImageData()
{
  return NULL;
}

//----------------------------------------------------------------------------
void vtkMRMLVolumeDisplayNode::SetBackgroundImageData(vtkImageData* vtkNotUsed(imageData))
{
}

//----------------------------------------------------------------------------
vtkImageData* vtkMRMLVolumeDisplayNode::GetBackgroundImageData()
{
  return 0;
}

//----------------------------------------------------------------------------
vtkImageData* vtkMRMLVolumeDisplayNode::GetOutputImageData()
{
  return NULL;
}

//----------------------------------------------------------------------------
void vtkMRMLVolumeDisplayNode::UpdateImageDataPipeline()
{
}

//----------------------------------------------------------------------------
void vtkMRMLVolumeDisplayNode::SetDefaultColorMap()
{
  this->SetAndObserveColorNodeID("vtkMRMLColorTableNodeGrey");
}

//----------------------------------------------------------------------------
vtkMRMLVolumeNode* vtkMRMLVolumeDisplayNode::GetVolumeNode()
{
  return vtkMRMLVolumeNode::SafeDownCast(this->GetDisplayableNode());
}

//----------------------------------------------------------------------------
vtkImageData* vtkMRMLVolumeDisplayNode::GetUpToDateImageData()
{
  vtkImageData* imageData = this->GetImageData();
  if (!imageData)
    {
    return NULL;
    }
  imageData->Update();
  return imageData;
}

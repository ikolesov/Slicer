#include "itkWarpTransform3D.h"

namespace itk
{

// Explicit New() method, used here because we need to split the itkNewMacro()
// in order to overload the CreateAnother() method.
// Explicit New() method, used here because we need to split the itkNewMacro()
// in order to overload the CreateAnother() method.
template <class FieldData>
typename WarpTransform3D<FieldData>::Pointer
WarpTransform3D<FieldData>
::New(void)
{
  Pointer smartPtr = ::itk::ObjectFactory<Self>::Create();

  if( smartPtr.IsNull() )
    {
    smartPtr = static_cast<Pointer>( new Self );
    }
  smartPtr->UnRegister();
  return smartPtr;
}

// Explicit New() method, used here because we need to split the itkNewMacro()
// in order to overload the CreateAnother() method.
template <class FieldData>
::itk::LightObject::Pointer
WarpTransform3D<FieldData>
::CreateAnother(void) const
{
  ::itk::LightObject::Pointer smartPtr;
  Pointer copyPtr = Self::New().GetPointer();

  copyPtr->SetDeformationField(this->GetDeformationField() );

  smartPtr = static_cast<Pointer>( copyPtr );

  return smartPtr;
}

template <class FieldData>
WarpTransform3D<FieldData>
::WarpTransform3D() :
#if ITK_VERSION_MAJOR >= 4
  Superclass( 1 )
#else
  Superclass( 3, 1 )
#endif
{
  m_DeformationField = 0;
//  m_OutputSpacing.Fill( 1 ) ;
  for( int i = 0; i < 3; i++ )
    {
    m_NeighborhoodRadius[i] = 1;    // radius of neighborhood we will use
    m_DerivativeWeights[i] = 1.0;
    }
  m_SizeForJacobian.Fill( 1 );
  this->m_NonThreadsafeSharedJacobian.SetSize( 3, 3 );
}

// Returns the position of the transformed point. If input point is outside of the deformation
// field, returns 0,0,0
template <class FieldData>
typename WarpTransform3D<FieldData>::OutputPointType
WarpTransform3D<FieldData>
::TransformPoint( const InputPointType & inputPoint ) const
{
  OutputPointType      transformedPoint;
  DeformationPixelType displacement;

  itk::Index<3> index;
  m_DeformationField->TransformPhysicalPointToIndex( inputPoint, index );
  if( !m_DeformationField->GetLargestPossibleRegion().IsInside( index ) )
    {
    return inputPoint;
    }
  displacement = m_DeformationField->GetPixel( index );
  transformedPoint = inputPoint + displacement;
  return transformedPoint;
}

// Copied and modified from dtiprocess
// available there: http://www.nitrc.org/projects/dtiprocess/
template <class FieldData>
const typename WarpTransform3D<FieldData>::JacobianType
& WarpTransform3D<FieldData>
::GetJacobian( const InputPointType &inputPoint ) const
  {
  this->ComputeJacobianWithRespectToParameters( inputPoint, this->m_NonThreadsafeSharedJacobian );
  return this->m_NonThreadsafeSharedJacobian;
  }

template <class FieldData>
void
WarpTransform3D<FieldData>
::ComputeJacobianWithRespectToParameters( const InputPointType & inputPoint, JacobianType & jacobian ) const
{

//  ZeroFluxNeumannBoundaryCondition< DeformationImageType > nbc;
  ConstNeighborhoodIteratorType bit;

  itk::ImageRegion<3> region;
  itk::Index<3>       start;
  m_DeformationField->TransformPhysicalPointToIndex( inputPoint, start );
  jacobian.SetSize( 3, 3 );
  if( !m_DeformationField->GetLargestPossibleRegion().IsInside( start ) )
    {
    jacobian.Fill( 0 );
    }
  region.SetIndex( start );
  region.SetSize( m_SizeForJacobian );
  bit = ConstNeighborhoodIteratorType(m_NeighborhoodRadius, m_DeformationField, region );
//  bit.OverrideBoundaryCondition(&nbc);
  bit.GoToBegin();
  for( unsigned int i = 0; i < 3; ++i )
    {
    for( unsigned int j = 0; j < 3; ++j )
      {
      jacobian( j, i ) = m_DerivativeWeights[i]
        * 0.5 * ( bit.GetNext( i )[j] - bit.GetPrevious( i )[j] );
      }
    }
}

template <class FieldData>
void
WarpTransform3D<FieldData>
::SetDeformationField( DeformationImagePointerType deformationField )
{
  m_DeformationField = deformationField;
  for( unsigned int i = 0; i < 3; i++ )
    {
    if( deformationField->GetSpacing()[i] == 0.0 )
      {
      itkExceptionMacro(<< "Image spacing in dimension " << i << " is zero." );
      }
    m_DerivativeWeights[i] = 1.0 / deformationField->GetSpacing()[i];
    }
//    m_OutputSpacing = deformationField->GetSpacing() ;
}

} // end of namespace

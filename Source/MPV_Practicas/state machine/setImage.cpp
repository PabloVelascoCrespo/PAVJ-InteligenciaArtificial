#include "setImage.h"
#include "../AICharacter.h"

SetImage::SetImage(AAICharacter* inOwner, const char* path) :
  m_inOwner(inOwner),
  m_path(path)
{}

void SetImage::start()
{
  if (!m_inOwner)
  {
    return;
  }
  m_inOwner->SetImage(m_path);
}

#include "FieldManager.hh"

#include "MagneticField.hh"

FieldManager* FieldManager::s_instance = 0;

FieldManager::FieldManager() :
  m_field(0)
{
}

FieldManager::~FieldManager()
{
  delete m_field;
}

FieldManager* FieldManager::instance()
{
  if (!s_instance) s_instance = new FieldManager;
  return s_instance;
}

void FieldManager::setField(const MagneticField* field)
{
  if (m_field) delete m_field;
  m_field = field;
}
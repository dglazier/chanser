/*Run 1 file*/
HipoData::SetFile()
{
  HipoData::Init(){
    if() _dataType=static_cast<Short_t> (chanser::DataType::Sim);
    _eventInfo.SetCLAS12( _c12 );
    _runInfo.SetCLAS12( _c12 );
    clas12::connectDBs()
  }
}
FinalStateManager::LoadData();
FinalStateManager::LoadFinalState()
{
  FinalState::SetEventParticles(&_eventParts); //link to data
  FinalState::SetEventInfo(_data->GetEventInfo());
  FinalState::PostRead(){
    FinalState::InitTopoMan();
    FinalState::Define();
    FinalState::SetTopologies();
  }
}
FinalStateManager::ProcessAll()
{
  FinalStateManager::Init()
    {
      FinalState::Init()
	{
	  ActionManger::Configure();
	  ActionManger::PostConfigure();
	}

    }
  FinalStateManager::Notify()
    {
      FinalState::ChangeRun()
	{
	  ActionManger::ChangeRun(); //Data init, Actions configured
	}
    }

  while(_data->InitEvent()){ /*file event loop*/ }

  FinalStateManager::EndAndWrite();
  
}
    

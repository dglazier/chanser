/*Run with HipoPRocessor/PROOF*/
clas12root::HipoChain chain;
HipoProcessor processor(&chain);
TProof::Process(&processor);
{
  HipoProcessor::SlaveBegin()
    {

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

    HipoProcessor::ApplyOptions();
    
    }
  HipoProcessor::Process():HipoSelector::Process()
    {
      if(newFile)
	{
	  new clas12::clas12reader;
	  HipoProcessor::Notify()
	    {
	      HipoSelector::Notify();
	      HipoData::SetReader(){
		HipoData::Init(){
		  if() _dataType=static_cast<Short_t> (chanser::DataType::Sim);
		  _eventInfo.SetCLAS12( _c12 );
		  _runInfo.SetCLAS12( _c12 );
		  
		}
	      }
	      FinalStateManager::Init()
		{
		  FinalState::Init();
		}
	      
	    }
	}
      while(_c12->nextInRecord()==true){/*record event loop*/}
      
    }
  

/**
class WaitObj{
	private:
	int id;
	bool is_done;
	
	WaitObj(){ // disable default constructor
		//throw exceptions; 
	}
	
	public:
	WaitObj(SingeltonWaitingList swl){
		swl.getAvailableId()
		swl.addWaitObj(this) // does this work
		is_done = false;
	}
	
	int getId(){
		return id;
	}
	
	bool isDone(){
		return is_done;
	}
	
	void waitUntilDone(){
		while(isDone){
			sleep(1);
		}
	}
	**/
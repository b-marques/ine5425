class : Entity
{
	public:
    Entity();
    ~Entity();

 	private:
		enum Type{
			ONE = 0,
			TWO = 1
		};

 		Type type_;
 		int arrival_time_;
 		int service_time_;
 		int dispose_time_;
 		
}
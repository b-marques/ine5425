class : Server
{
  public:
    Server();
    ~Server();
  private:
  	enum Type{
			ONE = 0,
			TWO = 1
		};

		Type type_;	
    bool state_;

}
#ifndef SERVER_H
#define SERVER_H

class Server
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

#endif //SERVER_H
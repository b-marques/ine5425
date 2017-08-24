class : Client
{
  public:
    Client();
    ~Client();
  private:
    int _time_since_last_arrive;
    int _arrive_time;
    int _service_time;
    int _start_service_time;
    int _queue_time;
    int _leave_time;
}
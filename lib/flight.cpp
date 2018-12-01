#include "flight.hpp"
using namespace std;

Flight::Flight(int _id,string _aeropuerto_init,string _aeropuerto_fin,string _horaInicio,string _horaFin){
     id =_id;
     aeropuerto_init = _aeropuerto_init;
     aeropuerto_fin = _aeropuerto_fin;
     horaInicio = _horaInicio;
     horaFin = _horaFin;
};
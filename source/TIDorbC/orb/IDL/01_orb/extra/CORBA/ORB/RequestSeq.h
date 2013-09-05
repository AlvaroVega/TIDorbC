#include "CORBA/ORB.h" // Parent Inclusion 


#ifndef _CORBA_ORB_REQUEST_SEQ_H_ 
#define _CORBA_ORB_REQUEST_SEQ_H_


class RequestSeq {
	public:
	RequestSeq() : m_buffer(0), m_release(true), m_length(0), m_max_length(0) {}
	RequestSeq(CORBA::ULong max): m_buffer(0), m_release(true), m_length(0), m_max_length(max){}
	RequestSeq(CORBA::ULong max, CORBA::ULong length, CORBA::Request_ptr* data, CORBA::Boolean release = false)
	: m_buffer(data), m_release(release),	m_length(length),  m_max_length(max) {}
	RequestSeq(const RequestSeq& other);
	~RequestSeq();
	RequestSeq& operator=(const RequestSeq&);
	CORBA::ULong maximum() const { return m_max_length;}
	void length(CORBA::ULong v);
	CORBA::ULong length() const {return m_length;}
	CORBA::Request_ptr& operator[](CORBA::ULong index) {return m_buffer[index];}
	const CORBA::Request_ptr& operator[](CORBA::ULong index) const {return (const CORBA::Request_ptr&) m_buffer[index];}
	CORBA::Boolean release() const {return m_release;}
	void replace(CORBA::ULong max, CORBA::ULong length, CORBA::Request_ptr* data, CORBA::Boolean release = false);
	CORBA::Request_ptr* get_buffer(CORBA::Boolean orphan = false);
	const CORBA::Request_ptr* get_buffer() const; 
	static CORBA::Request_ptr* allocbuf(CORBA::ULong size);
	static void freebuf(CORBA::Request_ptr* buf);
	private:
	void allocbuf();
	void freebuf();
	CORBA::Request_ptr* m_buffer;
	CORBA::Boolean m_release;
    CORBA::ULong m_length;
	CORBA::ULong m_max_length; 	
};

typedef RequestSeq* RequestSeq_ptr;

typedef ::TIDorb::templates::SequenceT_var<RequestSeq, CORBA::Request_ptr> RequestSeq_var;
typedef ::TIDorb::templates::SequenceT_out<RequestSeq, CORBA::Request_ptr> RequestSeq_out;


#endif //_CORBA_ORB_REQUEST_SEQ_H_  

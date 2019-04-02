#include <stdio.h>
#include <iostream>
#include <thread>
#include <queue>
#include <map>
#include <chrono>

#include "boost/asio.hpp"


#include <unistd.h>


#include "testclass.h"

using namespace std;
using boost::asio::ip::tcp;

void testout(int n);

class session
	: public std::enable_shared_from_this<session>
{
public:
	session(tcp::socket socket)
		: socket_(std::move(socket))
	{
	}

	void start()
	{
		do_read();
	}

private:
	void do_read()
	{
		auto self(shared_from_this());
		socket_.async_read_some(boost::asio::buffer(data_, max_length),
			[this, self](boost::system::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				do_write(length);
			}
		});
	}

	void do_write(std::size_t length)
	{
		auto self(shared_from_this());
		boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
			[this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				do_read();
			}
		});
	}

	tcp::socket socket_;
	enum { max_length = 1024 };
	char data_[max_length];
};



struct dij_dis
{
	int id, dis;
	dij_dis(const int& _id, const int& _dis)
	{
		id = _id;
		dis = _dis;
		return;
	}
};

class dij_dis_cmp
{
	bool reverse;
public:
	dij_dis_cmp(const bool& revparam = true)
	{
		reverse = revparam;
	}
	bool operator() (const dij_dis& lhs, const dij_dis&rhs) const
	{
		if (reverse)
		{
			return (lhs.dis > rhs.dis);
		}
		else
		{
			return (lhs.dis < rhs.dis);
		}
	}
};

#define ROUTER_ID int
#define INF_DIS 0x3030303

int main(int argc, char **argv)
{

	
	cout << this_thread::get_id() << endl;
	/*
	testclass t;

	thread perfoirming = thread(testclass::fun1, &t);
	perfoirming.detach();



	t.endflag = true;
	*/


	//tcp::iostream s;


	//s.connect("localhost", "38330");


	//std::cout << "local: "
	//	<< s.socket().local_endpoint().address() << " " 
	//	<< s.socket().local_endpoint().port() << std::endl;

	//std::cout << "remote: "
	//	<< s.socket().remote_endpoint().address() << " "
	//	<< s.socket().remote_endpoint().port() << std::endl;



	//while (true)
	//{
	//	string t;
	//	cout << "1" << std::endl;
	//	std::cin >> t;
	//	s << t;
	//	cout << "2" << std::endl;
	//	s >> t;
	//	cout << "3" << std::endl;
	//	std::cout << t;
	//	cout << "4" << std::endl;
	//}

	map<ROUTER_ID, map<ROUTER_ID, int>> lsmap;
	lsmap[0][1] = 1;
	lsmap[1][2] = 5;
	lsmap[0][4] = 3;
	lsmap[4][3] = 2;
	lsmap[4][1] = 6;
	lsmap[1][3] = 2;
	lsmap[4][6] = 8;
	lsmap[6][3] = 2;
	lsmap[3][2] = 1;
	for (map<ROUTER_ID, map<ROUTER_ID, int>>::iterator i = lsmap.begin();
		i != lsmap.end();i++)
	{
		for (map<ROUTER_ID, int > ::iterator j = i->second.begin();
			j != i->second.end();j++)
		{
			lsmap[j->first][i->first] = j->second;
		}
	}
	
	chrono::steady_clock::time_point tp = chrono::steady_clock::now();

	map<ROUTER_ID, ROUTER_ID> route_table_constructing;
	route_table_constructing.clear();

	map<ROUTER_ID, map<ROUTER_ID, int>>::iterator ls_it;

	int rt_list_index = 0;
	vector<ROUTER_ID> router_list;
	map<ROUTER_ID, int> reverse_router_list;
	router_list.clear();
	reverse_router_list.clear();

	for (ls_it = lsmap.begin();ls_it != lsmap.end();ls_it++)
	{
		router_list.push_back(ls_it->first);
		reverse_router_list[ls_it->first] = rt_list_index;
		rt_list_index++;
	}

	//initialize a 2d array for distance look up, indexed lsdb
	vector<vector<int>> index_lsdb;
	index_lsdb = vector<vector<int>>(router_list.size(),
		vector<int>(router_list.size(), INF_DIS));
	//fill in distance data from lsdb
	for (map<ROUTER_ID, map<ROUTER_ID, int>>::iterator i = lsmap.begin();
		i != lsmap.end();i++)
	{
		for (map<ROUTER_ID, int > ::iterator j = i->second.begin();
			j != i->second.end();j++)
		{
			if (j->second < 65536)
			{
				index_lsdb[reverse_router_list[i->first]][reverse_router_list[j->first]] = j->second;
			}
		}
	}

	//index of self
	int ind_self = reverse_router_list[0];


	//INF_DIS for no prev 
	vector<int> prev;
	prev = vector<int>(router_list.size(), INF_DIS);
	//visited list
	vector<bool> visited;
	visited = vector<bool>(router_list.size(), false);
	//distance from source node
	vector<int> dis_list;
	//initialize all nodes as unreachable from source node(self)
	dis_list = vector<int>(router_list.size(), INF_DIS);

	//record for next hop 2d 
	vector<int> next_hop;
	next_hop = vector<int>(router_list.size(), INF_DIS);

	dis_list[ind_self] = 0;

	//initialize a priority queue
	priority_queue<dij_dis, vector<dij_dis>, dij_dis_cmp> vort_q;
	while (!vort_q.empty())
	{
		vort_q.pop();
	}
	vort_q.push(dij_dis(ind_self, dis_list[ind_self]));

	while (!vort_q.empty())
	{
		dij_dis v_cur = vort_q.top();
		vort_q.pop();
		if (visited[v_cur.id])
		{
			continue;
		}
		if (v_cur.dis >= INF_DIS)
		{
			//no reachable, no means to continue.
			break;
		}
		for (int ind_nei = 0;ind_nei < router_list.size();ind_nei++)
		{
			int alt_dis;
			if (ind_nei == v_cur.id
				|| index_lsdb[v_cur.id][ind_nei] >= INF_DIS
				|| visited[ind_nei])
			{
				//filter out some useless condition
				continue;
			}

			//alternative distance
			alt_dis = dis_list[v_cur.id] + index_lsdb[v_cur.id][ind_nei];

			if (alt_dis < dis_list[ind_nei])
			{
				//directly get next hop
				if (v_cur.id == ind_self)
				{
					next_hop[ind_nei] = ind_nei;
				}
				else
				{
					next_hop[ind_nei] = next_hop[v_cur.id];
				}
				dis_list[ind_nei] = alt_dis;
				prev[ind_nei] = v_cur.id;

				vort_q.push(dij_dis(ind_nei, alt_dis));
			}
		}
		visited[v_cur.id] = true;
	}

	//transfer index into ROUTER_ID;
	for (int ind_nexthop = 0;ind_nexthop < router_list.size();ind_nexthop++)
	{
		if (next_hop[ind_nexthop] != INF_DIS)
		{
			route_table_constructing[router_list[ind_nexthop]] = router_list[next_hop[ind_nexthop]];
		}
	}


	cout << "duration: " << chrono::duration_cast<chrono::nanoseconds>(chrono::steady_clock::now() - tp).count() << endl;

	for (map<ROUTER_ID, ROUTER_ID>::iterator rouit = route_table_constructing.begin();
		rouit != route_table_constructing.end();rouit++)
	{
		cout << rouit->first << " next hop: " << rouit->second << endl;
	}
	for (int ind_nexthop = 0;ind_nexthop < router_list.size();ind_nexthop++)
	{
		if (prev[ind_nexthop] != INF_DIS)
		{
			cout << router_list[ind_nexthop] << router_list[prev[ind_nexthop]] << endl;
		}
	}

	int tt;
	cin >> tt;
	return 0;

/*
	map<int, map<int, int>> t;
//	t[2] = map<int, int>();
	t[2][3] = 6;

	cout << t[2][3] << endl;
	
	int tt;
	cin >> tt;
	*/


	return 0;
}


void testout(int n)
{
	
	cout << "thread id is: " << this_thread::get_id() << endl;
	cout << n * 10 << endl;
	sleep(5);
	cout << "sleept for 5 sec" << endl;
}



#ifndef MEMBER_H
#define MEMBER_H

#include <string>
#include <vector>

using namespace std;

struct Member
{
    int id_member;
    string nama_member;
    string no_wa;
};

vector<Member> bacaMember();
bool cariMemberById(int id, Member &hasil);
void tampilkanDaftarMember();
void cetakLaporanMember();
void menuMember();

#endif

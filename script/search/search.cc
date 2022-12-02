#include <string>
#include <vector>
#include <fstream>

std::string GccText = R"(s_as_acctoptpubflow.gcc   s_as_doptsettflow.gcc        s_as_noticeflow.gcc         s_as_optpubflow.gcc         s_as_oquryoptflow.gcc       s_as_soptplatflow.gcc     s_as_userpubflow.gcc      s_ls_ofilquryoptflow.gcc  s_ls_optreptflow.gcc       s_ls_quryoptpubflow.gcc
s_as_afilquryoptflow.gcc  s_as_extquryoptflow.gcc      s_as_ofilquryoptflow.gcc    s_as_optreportflow.gcc      s_as_oquryoptholdflow.gcc   s_as_svroptassetflow.gcc  s_ls_afilquryoptflow.gcc  s_ls_ohquryoptflow.gcc    s_ls_optriskflow.gcc       s_ls_svroptflow.gcc
s_as_ahquryoptflow.gcc    s_as_extuserflow.gcc         s_as_ohquryoptflow.gcc      s_as_optreptflow.gcc        s_as_oqurysecuordflow.gcc   s_as_svroptflow.gcc       s_ls_ahquryoptflow.gcc    s_ls_ooptplatflow.gcc     s_ls_optriskpubflow.gcc    s_ls_svrufkoptflow.gcc
s_as_aoptplatflow.gcc     s_as_foptsettflow.gcc        s_as_ooptplatflow.gcc       s_as_optriskflow.gcc        s_as_oquryuserflow.gcc      s_as_svroptuserflow.gcc   s_ls_aoptplatflow.gcc     s_ls_optargflow.gcc       s_ls_optsettflow.gcc       s_ls_uoptplatflow.gcc
s_as_aoptsettflow.gcc     s_as_fundoptpubflow.gcc      s_as_ooptsettflow.gcc       s_as_optsettflow.gcc        s_as_organoptplatflow.gcc   s_as_svruserflow.gcc      s_ls_aquryoptflow.gcc     s_ls_optbopflow.gcc       s_ls_optsubmitflow.gcc
s_as_aquryoptflow.gcc     s_as_fundpubflow.gcc         s_as_optargflow.gcc         s_as_optsubmitflow.gcc      s_as_organplatuserflow.gcc  s_as_sysoptpubflow.gcc    s_ls_extapioptflow.gcc    s_ls_optdailyflow.gcc     s_ls_opttradeflow.gcc
s_as_assetoptpubflow.gcc  s_as_fundsettflow.gcc        s_as_optdailyflow.gcc       s_as_optsync06crdtflow.gcc  s_as_organqryflow.gcc       s_as_syspubflow.gcc       s_ls_extassetoptflow.gcc  s_ls_optholdflow.gcc      s_ls_optutodbflow.gcc
s_as_assetplatflow.gcc    s_as_hisplatflow.gcc         s_as_optencryptionflow.gcc  s_as_optsyncflow.gcc        s_as_pollingoptflow.gcc     s_as_uoptplatflow.gcc     s_ls_extoptflow.gcc       s_ls_optpluginflow.gcc    s_ls_oquryoptflow.gcc
s_as_assetpubflow.gcc     s_as_hoptplatflow.gcc        s_as_optholdflow.gcc        s_as_opttradeflow.gcc       s_as_quryoptpubflow.gcc     s_as_uoptsettflow.gcc     s_ls_extquryoptflow.gcc   s_ls_optpubflow.gcc       s_ls_oquryoptholdflow.gcc
s_as_assetsyncflow.gcc    s_as_icsoptunitreptflow.gcc  s_as_optnoticeflow.gcc      s_as_oquryassetflow.gcc     s_as_secupubflow.gcc        s_as_useroptpubflow.gcc   s_ls_hoptplatflow.gcc     s_ls_optreportflow.gcc    s_ls_organoptplatflow.gcc)";

std::ofstream _out("Make.txt");

std::vector<std::string> ParseStringGcc(const std::string& str)
{
    bool flag = false;
    std::string gcc;
    std::vector<std::string> vec;
    for(const char c:str)
    {
        if(c==' ')
        {
            if(flag)
            {
                vec.push_back(gcc);
                gcc.clear();
                flag = false;
            }

        }
        else
        {
            flag = true;
            gcc.push_back(c);
        }
        
    }
    for(auto& s:vec)
    {
        s.insert(0, "make -f ");
        s.append(" ORA_VER=10\n");
    }
    return vec;
}

int main(int argc,char* argv[])
{
    std::vector<std::string> result(ParseStringGcc(GccText));
    for(const auto& v:result)
    {
        _out << v;
    }
    return 0;
}
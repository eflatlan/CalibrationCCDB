#if !defined(__CLING__) || defined(__ROOTCLING__)
#include "CCDB/BasicCCDBManager.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "CCDB/CCDBTimeStampUtils.h"
#include <iostream>
#include <fstream>
#endif


void testCcdbReader(const char* ccdbURI = "http://ccdb-test.cern.ch:8080", long timeStamp = 0)
{
  auto& ccdbMgr = o2::ccdb::BasicCCDBManager::instance();
  ccdbMgr.setURL(ccdbURI);
  
  if (!ccdbMgr.isHostReachable()) {
    std::cerr << ccdbURI << " is not reachable!" << std::endl;
    return;
  }
  if (timeStamp == 0) {
    timeStamp = o2::ccdb::getCurrentTimestamp();
  }
  
  
  auto firstTimeStamp = timeStamp -  300 * o2::ccdb::CcdbObjectInfo::DAY;
	auto gotResult = false;
  auto ts = timeStamp -  3 * o2::ccdb::CcdbObjectInfo::DAY;

  while(! gotResult &&  ts < timeStamp) {
  
		LOGP(info,  "==========================================");
		LOGP(info,  "==========================================");
  	ts += o2::ccdb::CcdbObjectInfo::DAY;
    ccdbMgr.setTimestamp(ts);
		// o2::cpv::Pedestals* peds = ccdbMgr.get<o2::cpv::Pedestals>("CPV/Calib/Pedestals");
				LOGP(info,  "Reading HMP/Calib/ChargeCut");
		std::vector<TF1, std::allocator<TF1>>* mChargeCut = ccdbMgr.get<std::vector<TF1, std::allocator<TF1>>>("HMP/Calib/ChargeCut");
		//   std::vector<TF1, std::allocator<TF1>>* mChargeCut = api.retrieveFromTFileAny<std::vector<TF1, std::allocator<TF1>>>("HMP/Calib/ChargeCut", metadata, ts);


	 // std::vector<TF1, std::allocator<TF1>>* mRefIndex = api.retrieveFromTFileAny<std::vector<TF1, std::allocator<TF1>>>("HMP/Calib/RefIndex", metadata, ts);

		LOGP(info,  "Reading HMP/Calib/RefIndex");
		  
		 //sstd::vector<TF1, std::allocator<TF1>>* mChargeCut = api.retrieveFromTFileAny<std::vector<TF1, std::allocator<TF1>>>("HMP/Calib/ChargeCut", metadata, ts);
	 
	 
	 
		std::vector<TF1, std::allocator<TF1>>* mRefIndex = ccdbMgr.get<std::vector<TF1, std::allocator<TF1>>>("HMP/Calib/RefIndex"); 
		int cnt = 0;
		if(!mRefIndex) {
			LOGP(info, "not found ts");
			continue;
		}
		
		gotResult = true;
		for (TF1& tf1 : *mChargeCut) {
		  std::unique_ptr<TCanvas> cChargeCut = std::make_unique<TCanvas>(Form("CCDB_ChargeCut number %i", cnt), Form("CCDB_ChargeCut number %i", cnt), 1200, 400);
		  tf1.Draw();
		  
		 // cChargeCut->SaveAs(Form("/root/hmpidTest/img/HV/ChargeCut number %i.eps", cnt));
		  cChargeCut->SaveAs(Form("/root/hmpidTest/img/HV/ChargeCut_number _%i.png", cnt));
		  cChargeCut->SaveAs(Form("/root/hmpidTest/img/HV/CCDB_ChargeCut_number_%i.eps", cnt));
		  cnt++;
		}

		// std::vector<TF1, std::allocator<TF1>>* mRefIndex = api.retrieveFromTFileAny<std::vector<TF1, std::allocator<TF1>>>("HMP/Calib/RefIndex", metadata, ts);
		// std::cout << "size of mRefIndex = " << mRefIndex->size() << std::endl;

		cnt = 0;
		for (TF1& tf1 : *mRefIndex) {
		  std::unique_ptr<TCanvas> cRefIndex = std::make_unique<TCanvas>(Form("CCDB_RefIndex number %i", cnt), Form("CCDB_RefIndex number %i", cnt), 1200, 400);
		  tf1.Draw();
		  cRefIndex->SaveAs(Form("/root/hmpidTest/img/HV/RefIndex_number_%i.eps", cnt));
		  cRefIndex->SaveAs(Form("/root/hmpidTest/img/HV/RefIndex_number_%i.png", cnt));
		  //cRefIndex->SaveAs(Form("/root/hmpidTest/img/HV/CCDB_RefIndex number %i.eps", cnt));
		  cnt++;
		}
		
	}

}

namespace chanser{

  namespace mva{

    namespace train{

      void GetMethodName( TString & name, TKey * mkey ) {
	if (mkey==0) return;
	name = mkey->GetName();
	name.ReplaceAll("Method_","");
      }
      
      void GetMethodTitle( TString & name, TKey * ikey ) {
	if (ikey==0) return;
	name = ikey->GetName();
      }
      inline Int_t getCanvas         () {return  TColor::GetColor( "#f0f0f0" );}
      inline Int_t getFrameFill      () {return  TColor::GetColor( "#fffffd" );}
      inline Int_t getTitleBox       () {return  TColor::GetColor( "#5D6B7D" );}
      inline Int_t getTitleBorder    () {return  TColor::GetColor( "#7D8B9D" );}
      inline Int_t getTitleText      () {return  TColor::GetColor( "#FFFFFF" );}
      inline Int_t getSignalLine     () {return  TColor::GetColor( "#0000ee" );}
      inline Int_t getSignalFill     () {return  TColor::GetColor( "#7d99d1" );}
      inline Int_t getBackgroundLine () {return  TColor::GetColor( "#ff0000" );}
      inline Int_t getBackgroundFill () {return  TColor::GetColor( "#ff0000" );}
      inline Int_t getNovelBlue      () {return  TColor::GetColor( "#2244a5" );}
  
      void SetSignalAndBackgroundStyle( TH1* sig, TH1* bkg, TH1* all=nullptr ) 
      {
	//signal
	// const Int_t FillColor__S = 38 + 150; // change of Color Scheme in ROOT-5.16.
	// convince yourself with gROOT->GetListOfColors()->Print()
	Int_t FillColor__S = getSignalFill();
	Int_t FillStyle__S = 1001;
	Int_t LineColor__S = getSignalLine();
	Int_t LineWidth__S = 2;

	// background
	//Int_t icolor = gConfig().fVariablePlotting.fUsePaperStyle ? 2 + 100 : 2;
	Int_t FillColor__B = getBackgroundFill();
	Int_t FillStyle__B = 3554;
	Int_t LineColor__B = getBackgroundLine();
	Int_t LineWidth__B = 2;

	if (sig != NULL) {
	  sig->SetLineColor( LineColor__S );
	  sig->SetLineWidth( LineWidth__S );
	  sig->SetFillStyle( FillStyle__S );
	  sig->SetFillColor( FillColor__S );
	}
 
	if (bkg != NULL) {
	  bkg->SetLineColor( LineColor__B );
	  bkg->SetLineWidth( LineWidth__B );
	  bkg->SetFillStyle( FillStyle__B );
	  bkg->SetFillColor( FillColor__B );
	}

	if (all != NULL) {
	  all->SetLineColor( LineColor__S );
	  all->SetLineWidth( LineWidth__S );
	  all->SetFillStyle( FillStyle__S );
	  all->SetFillColor( FillColor__S );
	}
      }
      
      void NormalizeHists( TH1* sig, TH1* bkg ) 
      {
	if (sig->GetSumw2N() == 0) sig->Sumw2();
	if (bkg && bkg->GetSumw2N() == 0) bkg->Sumw2();
      
	if(sig->GetSumOfWeights()!=0) {
	  Float_t dx = (sig->GetXaxis()->GetXmax() - sig->GetXaxis()->GetXmin())/sig->GetNbinsX();
	  sig->Scale( 1.0/sig->GetSumOfWeights()/dx );
	}
	if (bkg != 0 && bkg->GetSumOfWeights()!=0) {
	  Float_t dx = (bkg->GetXaxis()->GetXmax() - bkg->GetXaxis()->GetXmin())/bkg->GetNbinsX();
	  bkg->Scale( 1.0/bkg->GetSumOfWeights()/dx );
	}
      }

      void SetFrameStyle( TH1* frame, Float_t scale=1.0 )
      {
	frame->SetLabelOffset( 0.012, "X" );// label offset on x axis
	frame->SetLabelOffset( 0.012, "Y" );// label offset on x axis
	frame->GetXaxis()->SetTitleOffset( 1.25 );
	frame->GetYaxis()->SetTitleOffset( 1.22 );
	frame->GetXaxis()->SetTitleSize( 0.045*scale );
	frame->GetYaxis()->SetTitleSize( 0.045*scale );
	Float_t labelSize = 0.04*scale;
	frame->GetXaxis()->SetLabelSize( labelSize );
	frame->GetYaxis()->SetLabelSize( labelSize );

	// global style settings
	gPad->SetTicks();
	gPad->SetLeftMargin  ( 0.108*scale );
	gPad->SetRightMargin ( 0.050*scale );
	gPad->SetBottomMargin( 0.120*scale  );
      }

    }
  }
}

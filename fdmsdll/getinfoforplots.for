	subroutine getinfoforplots(

c	inputs

     + tf1, ! logical status (true or false) of N v BU plot : plot 1
     + tf2, ! logical status of G/BU v Cooling Time plot : plot 2
     + tf3, ! logical status of N v G plot : plot 3
     + tf4, ! logical status of N/G v Cycle # : plot 4
     + tf5, ! logical status of "include good" button
     + tf6, ! logical status of "include suspect" button
     + tf7, ! logical status of "accept good" button
     + tf8, ! logical status of "plot most recent measurement"

     + idb,   ! integer, number of enteries in "database.csv"
     + mtype, ! integer array(10000) measurement type 1:cycle ver, 2:assembly ver
     + en,    ! real*4, array(10000) containing the database (DB) enrichments in %
     + lognog, ! real*4 array(10000) log10 of N/G
     + icycle, ! integer array(10000) cycle number
     + logbu,    ! real*4, array(10000) containing the DB log10 burnups in GWd/MT
     + logct,    ! real*4, array(10000) containing the DB log10 cooling times in years
     + logdcn,   ! real*4, array(10000) containing the DB log10 decay corrected total neutrons
     + loggamma, ! real*4, array(10000) containing the DB log10 gamma doses
	
     + imp,      ! integer, number of enteries in "meaasurementplan.csv"
     + mtypemp,  ! integer array(1000) measurement type 1:cycle ver, 2:assembly ver
     + enmp,     ! real*4, array(1000) containing the MP enrichments in %
     + lognogmp, ! real*4 array(1000) log10 of MP N/G
     + icyclemp, ! integer array(1000) MP cycle number
     + logbump,  ! real*4, array(1000) containing the MP log10 burnups in GWd/MT
     + logctmp,  ! real*4, array(1000) containing the MP log10 cooling times in years
     + logdcnmp, ! real*4, array(1000) containing the MP log10 decay corrected total neutrons
     + loggammamp, ! real*4, array(1000) containing the MP log10 gamma doses
     + mstatusmp, ! integer(4), array(1000) status of the measure plan entries
	           ! 0 is non measurement measurement
	           ! 1 is value for most recent measurement on input
	           ! 2 is value for most recent measurement on exit if no fits
	           ! 3 is value for most recent measurement on exit if good
	           ! 4 is value for most recent measurement on exit if suspect
	           ! 5 is value for measurement on exit if no fits
	           ! 6 is value for measurement on exit if good
	           ! 7 is value for measurement on exit if suspect
	           ! 8 is measurement is added to the DB

     + imr, ! integer, measurementplan index for the most recent measurement
	      ! if there is no most recent measurement, set this to zero

     + delta_en,delta_ct, ! real*4, comparison difference for enrichment and cooling time

c	outputs

     + xgr1,ygr1,xgr2,ygr2,xgr3,ygr3,xgr4,ygr4, ! real*4 array(10000) x and y coord. for gray DB points for plots 1,2,3,4
     + xbl1,ybl1,xbl2,ybl2,xbl3,ybl3,xbl4,ybl4, ! real*4 array(10000) x and y coord. for blue DB points
     + xgd1,ygd1,xgd2,ygd2,xgd3,ygd3,xgd4,ygd4, ! real*4 array(1000)  x and y coord. for "good" recent measurement points
     + xnu1,ynu1,xnu2,ynu2,xnu3,ynu3,xnu4,ynu4, ! real*4 array(1000)  x and y coord. for "neutral" recent measurement points
     + xsp1,ysp1,xsp2,ysp2,xsp3,ysp3,xsp4,ysp4, ! real*4 array(1000) as above but "suspect"
     + xmr1,ymr1,xmr2,ymr2,xmr3,ymr3,xmr4,ymr4, ! real*4 x and y coord. for the most recent measurement

     + igr1,ibl1, ! integer number of gray, blue points for plot1
     + igr2,ibl2, ! integer number of gray, blue points for plot2
     + igr3,ibl3, ! integer number of gray, blue points for plot3
     + igr4,ibl4, ! integer number of gray, blue points for plot4
     + igd,inu,isp,   ! integer number of good, neutral, suspect points

     + fit1,fit2,fit3, ! logicals, are fits possible in plots 1, 2, and 3

     + a1,b1, ! real*4 slope and intercept for fit in plot 1
     + syx21,sx21,meanx1, ! real*4 statistical values need to calculate prediction limits for plot 1
     + a2,b2,syx22,sx22,meanx2,a3,b3,syx23,sx23,meanx3, ! real*4 as above but for plots 2 and 3 
     + ifit1,ifit2,ifit3, ! integer number of points in the fits to plots 1, 2, and 3

     + p4x,p4s ! plot 4 (cycle verification) arrays x:average s:one-sigma i=cycle number 1 to 10

     + )

      !DEC$ ATTRIBUTES DLLEXPORT :: getinfoforplots

	implicit none

c	begin input variables

	logical tf1,tf2,tf3,tf4,tf5,tf6,tf7,tf8

	integer idb,mtype(10000),icycle(10000)
	real*4 en(10000),lognog(10000),logbu(10000),logct(10000)
	real*4 logdcn(10000),loggamma(10000)

	integer imp,mtypemp(1000),icyclemp(1000)
	real*4 enmp(1000),lognogmp(1000),logbump(1000),logctmp(1000)
	real*4 logdcnmp(1000),loggammamp(1000)
	integer(4) mstatusmp(1000)

	integer imr

	real*4 delta_en,delta_ct 

c	end input variables

c	begin output variables

      real*4 xgr1(10000),ygr1(10000),xgr2(10000),ygr2(10000)
	real*4 xgr3(10000),ygr3(10000),xgr4(10000),ygr4(10000)
	real*4 xbl1(10000),ybl1(10000),xbl2(10000),ybl2(10000)
	real*4 xbl3(10000),ybl3(10000),xbl4(10000),ybl4(10000)
	real*4 xgd1(1000),ygd1(1000),xgd2(1000),ygd2(1000)
	real*4 xgd3(1000),ygd3(1000),xgd4(1000),ygd4(1000)
	real*4 xnu1(1000),ynu1(1000),xnu2(1000),ynu2(1000)
	real*4 xnu3(1000),ynu3(1000),xnu4(1000),ynu4(1000)
	real*4 xsp1(1000),ysp1(1000),xsp2(1000),ysp2(1000)
	real*4 xsp3(1000),ysp3(1000),xsp4(1000),ysp4(1000)
	real*4 xmr1,ymr1,xmr2,ymr2,xmr3,ymr3,xmr4,ymr4

	integer igr1,ibl1,igr2,ibl2,igr3,ibl3,igr4,ibl4,igd,inu,isp

	logical fit1,fit2,fit3

	real*4 a1,b1,syx21,sx21,meanx1
	real*4 a2,b2,syx22,sx22,meanx2,a3,b3,syx23,sx23,meanx3
	integer ifit1,ifit2,ifit3

	real*4 p4x(10),p4s(10)
	 
c	end output variables

c	begin internal variables

	integer i,j,n,cyclepoint
	real*4 xfit(10000),yfit(10000),erra,errb
	real*4 x,y
	logical goodpoint

c	end internal variables

c	Beginning of the subroutine

	do i=1,imp
	 if (mstatusmp(i).ge.1.and.mstatusmp(i).le.4) then
	  if (tf4) then
	   if (mtypemp(i).eq.1) then
	    imr=i
	    goto 10
	   endif
	  else
	   if (mtypemp(i).eq.2) then
	    imr=i
	    goto 10
	   endif
	  endif
	 endif
	enddo
	imr=0

10	if (tf4) then

	 igr4=0
	 ibl4=0
	 do i=1,idb
	  if (mtype(i).eq.1) then
	   ibl4=ibl4+1
	   xbl4(ibl4)=icycle(i)
	   ybl4(ibl4)=lognog(i)
	  endif
	 enddo

	 do j=1,10
	  p4x(j)=0.0
	  p4s(j)=0.0
	  n=0
	  do i=1,idb
	   if (mtype(i).eq.1) then
	    if (icycle(i).eq.j) then
	     n=n+1
	     p4x(j)=p4x(j)+lognog(i)
	     p4s(j)=p4s(j)+lognog(i)**2
	    endif
	   endif
	  enddo
	  if (n.gt.2) then
	   p4x(j)=p4x(j)/real(n)
	   p4s(j)=sqrt((p4s(j)-real(n)*p4x(j)**2)/real(n-1))
	  else
	   p4x(j)=0.0
	   p4s(j)=-1.0
	  endif
	 enddo

	 igd=0
	 inu=0
	 isp=0

	 if (imr.gt.0.and.mtypemp(imr).eq.1) then
	  xmr4=icyclemp(imr)
	  ymr4=lognogmp(imr)
	  mstatusmp(imr)=cyclepoint(xmr4,ymr4,p4x,p4s)
	 endif

	 do i=1,imp
	  if (mtypemp(i).eq.1) then
	   if (mstatusmp(i).ge.5.and.mstatusmp(i).le.7) then
	    x=icyclemp(i)
 	    y=lognogmp(i)
	    mstatusmp(i)=cyclepoint(x,y,p4x,p4s)+3
	    if (mstatusmp(i).eq.6) then
	     igd=igd+1
	     xgd4(igd)=icyclemp(i)
	     ygd4(igd)=lognogmp(i)
	    elseif (mstatusmp(i).eq.7) then
	     isp=isp+1
	     xsp4(isp)=icyclemp(i)
	     ysp4(isp)=lognogmp(i)
	    else
	     inu=inu+1
	     xnu4(inu)=icyclemp(i)
	     ynu4(inu)=lognogmp(i)
	    endif
	   endif
	  endif
	 enddo

	else

	 fit1=.false.
       fit2=.false.
	 fit3=.false.

	 igr1=0
	 ibl1=0
	 ifit1=0

	 if (tf1) then

	  if (imr.eq.0.or.mtypemp(imr).ne.2) then
	   do i=1,idb
	    if (mtype(i).eq.2) then
	     xgr1(i)=logbu(i)
	     ygr1(i)=logdcn(i)
	     igr1=igr1+1
	    endif
	   enddo
	  else
	   do i=1,idb
	    if (mtype(i).eq.2) then
	     if (abs(en(i)-enmp(imr)).le.delta_en) then
	      ibl1=ibl1+1
	      xbl1(ibl1)=logbu(i)
	      ybl1(ibl1)=logdcn(i)
	      ifit1=ifit1+1
	      xfit(ifit1)=logbu(i)
	      yfit(ifit1)=logdcn(i)
	     else
	      igr1=igr1+1
	      xgr1(igr1)=logbu(i)
	      ygr1(igr1)=logdcn(i)
	     endif
	    endif
	   enddo
	   if (ifit1.gt.2) then
	    fit1=.true.
	    call getlinearfit(ifit1,xfit,yfit,a1,b1,
     +                      erra,errb,syx21,sx21,meanx1)
	   endif
	  endif

	 endif

	 igr2=0
	 ibl2=0
	 ifit2=0

	 if (tf2) then

	  do i=1,idb
	   if (mtype(i).eq.2) then
	    ibl2=ibl2+1
	    xbl2(ibl2)=logct(i)
	    ybl2(ibl2)=loggamma(i)-logbu(i)
	    ifit2=ifit2+1
	    xfit(ifit2)=logct(i)
	    yfit(ifit2)=loggamma(i)-logbu(i)
	   endif
	  enddo
	  if (ifit2.gt.2) then
	   fit2=.true.
	   call getlinearfit(ifit2,xfit,yfit,a2,b2,
     +                      erra,errb,syx22,sx22,meanx2)
	  else
	   a2=0
	  endif

	 endif

	 igr3=0
	 ibl3=0
	 ifit3=0

	 if (tf3) then

	  ifit2=0
	  do i=1,idb
	   if (mtype(i).eq.2) then
	    xfit(i)=logct(i)
	    yfit(i)=loggamma(i)-logbu(i)
	    ifit2=ifit2+1
	   endif
	  enddo
	  if (ifit2.gt.2) then
	   call getlinearfit(ifit2,xfit,yfit,a2,b2,
     +             erra,errb,syx22,sx22,meanx2)
	  else
	   a2=0
	  endif
	  if (imr.eq.0.or.mtypemp(imr).ne.2) then
	   do i=1,idb
	    if (mtype(i).eq.2) then
	     xgr3(i)=loggamma(i)-a2*(logct(i)-1)
	     ygr3(i)=logdcn(i)
	     igr3=igr3+1
	    endif
	   enddo
	  else
	   do i=1,idb
	    if (mtype(i).eq.2) then
	     if (abs(en(i)-enmp(imr)).le.delta_en.and.
     +         abs(10**(logct(i))-10**(logctmp(imr))).le.delta_ct) then
	      ibl3=ibl3+1
	      xbl3(ibl3)=loggamma(i)-a2*(logct(i)-1)
	      ybl3(ibl3)=logdcn(i)
	      ifit3=ifit3+1
	      xfit(ifit3)=loggamma(i)-a2*(logct(i)-1)
	      yfit(ifit3)=logdcn(i)
	     else
	      igr3=igr3+1
	      xgr3(igr3)=loggamma(i)-a2*(logct(i)-1)
	      ygr3(igr3)=logdcn(i)
	     endif
	    endif
	   enddo
	   if (ifit3.gt.2) then
	    fit3=.true.
	    call getlinearfit(ifit3,xfit,yfit,a3,b3,
     +                      erra,errb,syx23,sx23,meanx3)
	   endif
	  endif

	 endif

	 igd=0
	 inu=0
	 isp=0

	 if (imr.gt.0.and.mtypemp(imr).eq.2) then
	  mstatusmp(imr)=2
	  if (fit1.or.fit2.or.fit3) mstatusmp(imr)=3
	  if (fit1) then
	   xmr1=logbump(imr)
	   ymr1=logdcnmp(imr)
	   if (.not.goodpoint(xmr1,ymr1,a1,b1,
     +         ifit1,syx21,sx21,meanx1)) mstatusmp(imr)=4
	  endif
	  if (fit2) then
	   xmr2=logctmp(imr)
	   ymr2=loggammamp(imr)-logbump(imr)
	   if (.not.goodpoint(xmr2,ymr2,a2,b2,
     +         ifit2,syx22,sx22,meanx2)) mstatusmp(imr)=4
	  endif
	  if (fit3) then
	   xmr3=loggammamp(imr)-a2*(logctmp(imr)-1)
	   ymr3=logdcnmp(imr)
	   if (.not.goodpoint(xmr3,ymr3,a3,b3,
     +         ifit3,syx23,sx23,meanx3)) mstatusmp(imr)=4
	  endif
	 endif

	 do i=1,imp
	  if (mtypemp(i).eq.2) then
	   if (mstatusmp(i).ge.5.and.mstatusmp(i).le.7) then
	    mstatusmp(i)=6
	    if (fit1) then
	     if (abs(enmp(i)-enmp(imr)).gt.delta_en) then
	      mstatusmp(i)=5
	     endif
	    endif
	    if (fit3) then
	     if (abs(enmp(i)-enmp(imr)).gt.delta_en.or.
     +       abs(10**(logctmp(i))-10**(logctmp(imr))).gt.delta_ct) then
	      mstatusmp(i)=5
	     endif
	    endif
	    if(.not.fit1.and.(.not.fit2).and.(.not.fit3)) mstatusmp(i)=5
	    if(mstatusmp(i).ne.5) then
	     if (fit1) then
	      x=logbump(i)
 	      y=logdcnmp(i)
	      if (.not.goodpoint(x,y,a1,b1,
     +         ifit1,syx21,sx21,meanx1)) mstatusmp(i)=7
	     endif
	     if (fit2) then
	     x=logctmp(i)
	      y=loggammamp(i)-logbump(i)
	      if (.not.goodpoint(x,y,a2,b2,
     +          ifit2,syx22,sx22,meanx2)) mstatusmp(i)=7
	     endif
	     if (fit3) then
	      x=loggammamp(i)-a2*(logctmp(i)-1)
	      y=logdcnmp(i)
	      if (.not.goodpoint(x,y,a3,b3,
     +         ifit3,syx23,sx23,meanx3)) mstatusmp(i)=7
	     endif
	    endif
	    if (mstatusmp(i).eq.6) then
	     igd=igd+1
	     xgd1(igd)=logbump(i)
	     ygd1(igd)=logdcnmp(i)
	     xgd2(igd)=logctmp(i)
	     ygd2(igd)=loggammamp(i)-logbump(i)
	     xgd3(igd)=loggammamp(i)-a2*(logctmp(i)-1)
	     ygd3(igd)=logdcnmp(i)
	    elseif (mstatusmp(i).eq.7) then
	     isp=isp+1
	     xsp1(isp)=logbump(i)
	     ysp1(isp)=logdcnmp(i)
	     xsp2(isp)=logctmp(i)
	     ysp2(isp)=loggammamp(i)-logbump(i)
	     xsp3(isp)=loggammamp(i)-a2*(logctmp(i)-1)
	     ysp3(isp)=logdcnmp(i)
	    else
	     inu=inu+1
	     xnu1(inu)=logbump(i)
	     ynu1(inu)=logdcnmp(i)
	     xnu2(inu)=logctmp(i)
	     ynu2(inu)=loggammamp(i)-logbump(i)
	     xnu3(inu)=loggammamp(i)-a2*(logctmp(i)-1)
	     ynu3(inu)=logdcnmp(i)
	    endif
	   endif
	  endif
	 enddo

	endif

	end

c	*******************************************************************************

	function cyclepoint(x,y,av,s)
	implicit none
	integer i,n
	real*4 x,y,av(10),s(10),stvalue,ypl1,ypl2
	integer cyclepoint

	stvalue=3.0
	i=int(x+0.1)
	if (s(i).gt.0.0) then
	 ypl1=av(i)-stvalue*s(i)
	 ypl2=av(i)+stvalue*s(i)
	 if (y.gt.ypl2.or.y.lt.ypl1) then
	  cyclepoint=4
	 else
	  cyclepoint=3
	 endif
	else
	 cyclepoint=2
	endif
	end
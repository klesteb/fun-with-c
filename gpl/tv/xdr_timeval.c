
/*******************************************************************************

Procedure:

    xdr_timeval ()


Purpose:

    Function xdr_timeval() is an XDR(3)-compatible function that
    encodes/decodes a UNIX "timeval" structure into/from XDR format.


    Invocation:

        successful = xdr_timeval (xdrStream, &time) ;

    where

        <xdrStream>     - I
            is the XDR stream handle returned by one of the xdrTTT_create()
            functions.
        <time>          - I/O
            is a UNIX "timeval" structure.
        <successful>    - O
            returns TRUE if the XDR translation was successful and FALSE
            if it was not.

*******************************************************************************/

/*
bool_t  xdr_timeval (

#    if __STDC__
        XDR  *xdrStream,
        struct  timeval  *time)
#    else
        xdrStream, time)

        XDR  *xdrStream ;
        struct  timeval  *time ;
#    endif

{

    if (!xdr_long(xdrStream, &time->tv_sec))  return(FALSE) ;
    if (!xdr_long(xdrStream, &time->tv_usec))  return(FALSE) ;

    return(TRUE) ;

}
*/

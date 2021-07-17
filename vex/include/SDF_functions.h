// $HOME/vex/include

function float smooth_step( float edge0; float edge1; float x; )
{
    float p = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    float v = p * p * (3.0 - 2.0 * p);
    
    return v;
}

float ndot(vector2 a; vector2 b; ) { return a.x*b.x - a.y*b.y; }

function float sdCircle( vector2 p; float r; )
{
    return length(p) - r;
}

function float sdRoundedBox( vector2 p; vector2 b; vector4 r; )
{
    r.x  = (p.y>0.0)?r.x  : r.y;
    vector2 q = abs(p)-b+r.x;
    return min(max(q.x,q.y),0.0) + length(max(q,0.0)) - r.x;
}

function float sdBox( vector2 p; vector2 b; )
{
    vector2 d = abs(p)-b;
    return length(max(d,0.0)) + min(max(d.x,d.y),0.0);
}

function float sdOrientedBox( vector2 p; vector2 a; vector2 b; float th; )
{
    float l = length(b-a);
    vector2  d = (b-a)/l;
    vector2  q = (p-(a+b)*0.5);
          q = matrix2(set(set(d.x,-d.y), set(d.y,d.x) ) ) * q;
          q = abs(q)-set(l,th)*0.5;
    return length(max(q,0.0)) + min(max(q.x,q.y),0.0);
}

function float sdSegment( vector2 p; vector2 a; vector2 b; )
{
    vector2 pa = p-a, ba = b-a;
    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
    return length( pa - ba*h );
}

function float sdRhombus( vector2 p; vector2 b; ) 
{
    vector2 q = abs(p);
    float h = clamp((-2.0*ndot(q,b)+ndot(b,b))/dot(b,b),-1.0,1.0);
    float d = length( q - 0.5*b*set(1.0-h,1.0+h) );
    return d * sign( q.x*b.y + q.y*b.x - b.x*b.y );
}

function float sdEquilateralTriangle( vector2 p; float r )
{
    float k = sqrt(3.0);
    p.x = abs(p.x) - r;
    p.y = p.y + r/k;
    if( p.x+k*p.y>0.0 ) p = set(p.x-k*p.y, -k*p.x-p.y) / 2.0;
    p.x -= clamp( p.x, -2.0*r, 0.0 );
    return -length(p)*sign(p.y);
}

function float sdBlobbyCross( vector2 pos; float he )
{
    pos = abs(pos);
    pos = set(abs(pos.x-pos.y),1.0-pos.x-pos.y)/sqrt(2.0);

    float p = (he-pos.y-0.25/he)/(6.0*he);
    float q = pos.x/(he*he*16.0);
    float h = q*q - p*p*p;
    
    float x;
    if( h>0.0 ) { float r = sqrt(h); x = pow(q+r,1.0/3.0)-pow(abs(q-r),1.0/3.0)*sign(r-q); }
    else        { float r = sqrt(p); x = 2.0*r*cos(acos(q/(p*r))/3.0); }
    x = min(x,sqrt(2.0)/2.0);
    
    vector2 z = set(x,he*(1.0-2.0*x*x)) - pos;
    return length(z) * sign(z.y);
}
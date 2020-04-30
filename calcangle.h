Vector CAimbot::CalcAngle( Vector& src, Vector& dst )
{
Vector vAngle;
Vector delta( (src.X-dst.X), (src.Y-dst.Y), (src.Z-dst.Z) );
double hyp = sqrt( delta.X*delta.X + delta.Y*delta.Y );
 
vAngle.X = (float)(asinf( (delta.Z + 64.06f) / hyp ) * 57.295779513082f);
vAngle.Y = (float)(atanf( delta.Y / delta.X ) * 57.295779513082f);
vAngle.Z = 0.0f;
 
if(delta.X >= 0.0)
vAngle.Y += 180.0f;
 
return vAngle;
}


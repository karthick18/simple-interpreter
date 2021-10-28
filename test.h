v = 5;
res = fact (v);
printf ("fact of %d = %d\n", v, res);
fun fact (a)
{
  if (!a || a == 1)
    {
      return 1;
    }
  return a * fact (a - 1);
}

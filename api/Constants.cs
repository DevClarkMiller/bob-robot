namespace BobRobotApi;

public static class Constants
{
	public static class Roles
	{
		public const string Admin = "Admin";
		public const string Chatter = "Chatter";
		public const string Unit = "Unit";
	}

	public static class Token
	{
		public const int TokenLifetime = 99999; // Means token lives forever
	}
}
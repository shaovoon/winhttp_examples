using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace WebApplication1
{
    public partial class _Default : Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (Page.IsPostBack)
                return;

            if (!string.IsNullOrEmpty(Request.QueryString["SearchTerm"]))
            {
                string searchTerm = Request.QueryString["SearchTerm"];
                Response.Write(searchTerm);
                Response.End();
            }
        }
    }
}
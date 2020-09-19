using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Web.Http;
using RestWebApp.Models;

namespace RestWebApp.Controllers
{
    public class ProductsController : ApiController
    {
        static Dictionary<long, Product> products = new Dictionary<long, Product>();

        [HttpGet]
        public List<Product> GetAll()
        {
            List<Product> temp = new List<Product>();
            foreach(var item in products)
            {
                temp.Add(item.Value);
            }
            return temp;
        }

        [HttpGet]
        public IHttpActionResult GetProduct(long id)
        {
            try
            {
                return Ok(products[id]);
            }
            catch (System.Collections.Generic.KeyNotFoundException)
            {
                return NotFound();
            }
        }
        [HttpPost]
        public IHttpActionResult Create([FromBody] Product item)
        {
            if (item == null)
            {
                return BadRequest();
            }

            products[item.Id] = item;

            return Ok();
        }
        [HttpPut]
        public IHttpActionResult Update(long id, [FromBody] Product item)
        {
            if (item == null || item.Id != id)
            {
                return BadRequest();
            }

            if(products.ContainsKey(id)==false)
            {
                return NotFound();
            }
            var product = products[id];

            product.Name = item.Name;
            product.Qty = item.Qty;
            product.Price = item.Price;

            return Ok();
        }
        [HttpDelete]
        public IHttpActionResult Delete(long id)
        {
            var product = products[id];
            if (product == null)
            {
                return NotFound();
            }

            products.Remove(id);

            return Ok();
        }
    }
}
